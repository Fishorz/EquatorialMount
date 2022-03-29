#include <Arduino.h>
//請修改以下參數--------------------------------------------
char ssid[] = "SSID";
char password[] = "SSIDpassword";
//請修改為你自己的API Key，並將https改為http
String url = "http://api.thingspeak.com/update?api_key=換成你的APIKey";
int pinDHT11 = 14; //假設DHT11接在腳位GPIO14，麵包板左側序號8
//---------------------------------------------------------
SimpleDHT11 dht11(pinDHT11); //宣告SimpleDHT11物件

//公用變數區
byte temperature = 0;
byte humidity = 0;
bool SendFlag = false;
//宣告任務Task1
TaskHandle_t Task1;

//任務1副程式Task1_senddata
void Task1_senddata(void *pvParameters)
{
    //無窮迴圈
    for (;;)
    {
        //偵測上傳旗標是否為true
        if (SendFlag)
        {
            Serial.print("Task1：啟動網頁連線，at core:");
            Serial.println(xPortGetCoreID());
            HTTPClient http;
            //將溫度及濕度以http get參數方式補入網址後方
            String url1 = url + "&amp;field1=" + (int)temperature + "&amp;field2=" + (int)humidity;
            // http client取得網頁內容
            http.begin(url1);
            int httpCode = http.GET();
            if (httpCode == HTTP_CODE_OK)
            {
                //讀取網頁內容到payload
                String payload = http.getString();
                //將內容顯示出來
                Serial.print("網頁內容=");
                Serial.println(payload);
            }
            else
            {
                //傳送失敗
                Serial.println("網路傳送失敗");
            }
            //修改完畢，修改傳送旗標=false
            SendFlag = false;
            http.end();
        }
        else
        {
            // Task1休息，delay(1)不可省略
            delay(1);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.print("開始連線到無線網路SSID:");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("連線完成");
    //在核心0啟動任務1
    xTaskCreatePinnedToCore(
        Task1_senddata, / <em> 任務實際對應的Function</ em> /
                            "Task1",
        / <em> 任務名稱</ em> /
            10000,
        / <em> 堆疊空間</ em> /
            NULL,
        / <em> 無輸入值</ em> /
            0,
        / <em> 優先序0</ em> /
            &amp;
        Task1, / <em> 對應的任務變數位址</ em> /
                   0); /*指定在核心0執行 */
}

void loop()
{
    Serial.print("loop主流程：溫濕度讀取，at core:");
    Serial.println(xPortGetCoreID());
    //嘗試讀取溫濕度內容
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&amp; temperature, &amp; humidity, NULL)) != SimpleDHTErrSuccess)
    {
        Serial.print("溫度計讀取失敗，錯誤碼=");
        Serial.println(err);
        delay(1000);
        return;
    }
    //讀取成功，將溫濕度顯示在序列視窗
    Serial.print("溫度計讀取成功: ");
    Serial.print((int)temperature);
    Serial.print(" *C, ");
    Serial.print((int)humidity);
    Serial.println(" H");
    //修改上傳旗標=true
    SendFlag = true;
    delay(20000); //休息20秒
}
