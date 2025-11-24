#include <string.h>
#include <stdio.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_random.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define MAX_SSIDS 24
#define BASE_NAME "fake_ssid" // SSID base name (changable)
#define SSID_COUNT 21         // number of fake SSIDs (changable)

static char ssid_list[MAX_SSIDS][32];
static uint8_t ssid_macs[MAX_SSIDS][6];

typedef struct __attribute__((packed))
{
    uint8_t fc[2];
    uint8_t duration[2];
    uint8_t da[6];
    uint8_t sa[6];
    uint8_t bssid[6];
    uint8_t seq[2];
} wifi_mgmt_hdr_t;

typedef struct __attribute__((packed))
{
    wifi_mgmt_hdr_t hdr;
    uint8_t timestamp[8];
    uint8_t interval[2];
    uint8_t capability[2];

    uint8_t ssid_id;
    uint8_t ssid_len;
    char ssid[32];

    uint8_t rate_id;
    uint8_t rate_len;
    uint8_t rates[8];

    uint8_t ds_id;
    uint8_t ds_len;
    uint8_t channel;

} beacon_frame_t;

static void generate_ssids(void)
{
    static const uint8_t oui[5][3] = {
        {0x00, 0x1A, 0x11},
        {0x00, 0x24, 0x01},
        {0x00, 0x26, 0x5E},
        {0x00, 0x50, 0x56},
        {0x00, 0x1B, 0xFC}};

    for (int i = 0; i < SSID_COUNT; i++)
    {
        uint32_t rnd = esp_random() % 10000;

        char short_name[21];
        strncpy(short_name, BASE_NAME, sizeof(short_name) - 1);
        short_name[sizeof(short_name) - 1] = '\0';

        int n = snprintf(ssid_list[i], sizeof(ssid_list[i]), "%.*s_%04lu",
                         20, short_name, (unsigned long)rnd);
        if (n < 0 || n >= sizeof(ssid_list[i]))
            ssid_list[i][sizeof(ssid_list[i]) - 1] = '\0';

        memcpy(ssid_macs[i], oui[i % 5], 3);
        ssid_macs[i][3] = (esp_random() >> 24) & 0xFF;
        ssid_macs[i][4] = (esp_random() >> 16) & 0xFF;
        ssid_macs[i][5] = (esp_random() >> 8) & 0xFF;

        ssid_macs[i][0] &= 0xFE;
    }
}

static void build_frame(beacon_frame_t *f, const char *ssid, const uint8_t *mac, uint8_t ch)
{
    memset(f, 0, sizeof(*f));

    f->hdr.fc[0] = 0x80;
    f->hdr.fc[1] = 0x00;

    memset(f->hdr.da, 0xFF, 6);
    memcpy(f->hdr.sa, mac, 6);
    memcpy(f->hdr.bssid, mac, 6);

    f->interval[0] = 0x64;
    f->interval[1] = 0x00;

    f->capability[0] = 0x01;
    f->capability[1] = 0x00;

    uint8_t len = strlen(ssid);
    f->ssid_id = 0x00;
    f->ssid_len = len;
    memcpy(f->ssid, ssid, len);

    f->rate_id = 0x01;
    f->rate_len = 8;
    uint8_t r[8] = {0x82, 0x84, 0x8B, 0x96, 0x24, 0x30, 0x48, 0x6C};
    memcpy(f->rates, r, 8);

    f->ds_id = 0x03;
    f->ds_len = 1;
    f->channel = ch;
}

static void wifi_init_fast(void)
{
    esp_netif_init();
    esp_event_loop_create_default();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    esp_wifi_set_promiscuous(true);
}

static void spam_task(void *arg)
{
    generate_ssids();
    wifi_init_fast();

    beacon_frame_t frame;
    uint8_t channel = 1;

    while (1)
    {
        esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);

        for (int i = 0; i < SSID_COUNT; i++)
        {
            build_frame(&frame, ssid_list[i], ssid_macs[i], channel);
            esp_wifi_80211_tx(WIFI_IF_STA, &frame, sizeof(frame), false);
            vTaskDelay(pdMS_TO_TICKS(5));
        }

        channel++;
        if (channel > 11)
            channel = 1;

        vTaskDelay(pdMS_TO_TICKS(150));
    }
}

void app_main(void)
{
    nvs_flash_init();
    xTaskCreate(spam_task, "spam", 4096, NULL, 5, NULL);
}
