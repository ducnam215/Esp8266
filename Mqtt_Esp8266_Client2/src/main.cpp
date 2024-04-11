#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <max7219.h>
#include <Wire.h>
#include <stdlib.h>

#define Btn_Red 5
#define Btn_Green 4

#define LEFT 0
#define RIGHT 1

long lastMsg = 0;
float temperature = 0;
float humidity = 0;

uint8_t Value_Red, Value_Green;
uint8_t Default_Red = 1, Default_Green = 1;
uint8_t Count_Red = 0, Count_Green = 0;

// Khai báo biến `ssid` là một con trỏ kiểu char, chứa tên của mạng WiFi mà bạn muốn kết nối.
const char *ssid = "Samsung Galaxy Z Flip4";
// Khai báo biến `password` là một con trỏ kiểu char, chứa mật khẩu của mạng WiFi mà bạn muốn kết nối.
const char *password = "241810697";
// Khai báo biến `mqtt_server` là một con trỏ kiểu char, chứa địa chỉ của máy chủ MQTT mà bạn muốn kết nối.
const char *mqtt_server = "valianthead152.cloud.shiftr.io";
// Khai báo biến `mqtt_port` kiểu int, chứa cổng kết nối đến máy chủ MQTT.
const int mqtt_port = 1883;
// Khai báo biến `mqtt_username` là một con trỏ kiểu char, chứa tên người dùng dùng để xác thực kết nối MQTT.
const char *mqtt_username = "valianthead152";
// Khai báo biến `mqtt_password` là một con trỏ kiểu char, chứa mật khẩu dùng để xác thực kết nối MQTT.
// Lưu ý: Mật khẩu ở đây đã được ẩn đi vì lí do bảo mật.
const char *mqtt_password = "test123";
// Khởi tạo đối tượng espClient từ class WiFiClient, sử dụng để thiết lập kết nối WiFi.
WiFiClient espClient;
// Khởi tạo đối tượng client từ class PubSubClient, sử dụng để gửi và nhận tin nhắn MQTT.
// Đối tượng này được khởi tạo với espClient làm tham số, điều này cho phép nó sử dụng kết nối WiFi đã thiết lập.
PubSubClient client(espClient);
MAX7219 max7219;

void setup_wifi()
{
    delay(10); // Dừng chương trình trong một khoảng thời gian ngắn (10 milliseconds). Điều này có thể giúp ổn định module WiFi trước khi bắt đầu quá trình kết nối.

    // Bắt đầu kết nối WiFi bằng cách in thông báo trên cổng Serial.
    Serial.println();               // In một dòng trống để tạo khoảng cách trong log.
    Serial.print("Connecting to "); // In thông điệp "Connecting to " để báo hiệu đang tiến hành kết nối WiFi.
    Serial.println(ssid);           // In tên mạng WiFi (SSID) mà bạn đang cố gắng kết nối.

    WiFi.begin(ssid, password); // Bắt đầu kết nối WiFi bằng cách sử dụng SSID và mật khẩu đã được cung cấp.

    while (WiFi.status() != WL_CONNECTED) // Kiểm tra xem thiết bị đã kết nối thành công với WiFi hay chưa.
    {
        delay(500);        // Nếu chưa kết nối, hãy chờ 500 milliseconds trước khi kiểm tra lại.
        Serial.print("."); // In dấu chấm "." lên cổng Serial để biểu thị quá trình chờ kết nối.
    }

    // Khi thiết bị đã kết nối thành công với WiFi:
    Serial.println("");               // In một dòng trống.
    Serial.println("WiFi connected"); // Thông báo "WiFi connected" qua cổng Serial.
    Serial.println("IP address: ");   // Thông báo sẽ in ra địa chỉ IP của thiết bị.
    Serial.println(WiFi.localIP());   // In địa chỉ IP của thiết bị trong mạng WiFi mà nó vừa kết nối.
}

void callback(char *topic, byte *message, unsigned int length)
{
    // In thông điệp nhận được ra cổng Serial để debug hoặc theo dõi.
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    // Vòng lặp chuyển đổi tin nhắn từ dạng byte sang String.
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)message[i]);  // In từng ký tự của tin nhắn.
        messageTemp += (char)message[i]; // Thêm từng ký tự vào chuỗi messageTemp.
    }
    Serial.println(); // Kết thúc dòng hiện tại trên cổng Serial.

    // Kiểm tra chủ đề của tin nhắn và thực hiện hành động tương ứng.
    if (strcmp(topic, "esp8266/client1/aht20/temperature") == 0)
    {
        const char *messageArray = messageTemp.c_str();
        // Kiểm tra độ dài của messageArray
        int messageLength = strlen(messageArray);
        // Đảm bảo là messageArray có độ dài nhỏ hơn hoặc bằng 6
        if (messageLength <= 4)
        {
            // Khai báo một mảng char[6] để lưu trữ messageArray
            char messageChar[4];
            // Sao chép nội dung từ messageArray sang messageChar
            strcpy(messageChar, messageArray);
            max7219.DisplayText(messageChar, LEFT);
        }
        else
        {
            // Xử lý trường hợp độ dài messageArray lớn hơn 6 nếu cần thiết
        }
    }

    // Làm tương tự như trên cho LED xanh lá.
    if (strcmp(topic, "esp8266/client1/aht20/humidity") == 0)
    {
        const char *messageArray = messageTemp.c_str();
        // Kiểm tra độ dài của messageArray
        int messageLength = strlen(messageArray);
        // Đảm bảo là messageArray có độ dài nhỏ hơn hoặc bằng 6
        if (messageLength <= 4)
        {
            // Khai báo một mảng char[6] để lưu trữ messageArray
            char messageChar[4];
            // Sao chép nội dung từ messageArray sang messageChar
            strcpy(messageChar, messageArray);
            max7219.DisplayText(messageChar, RIGHT);
        }
        else
        {
            // Xử lý trường hợp độ dài messageArray lớn hơn 6 nếu cần thiết
        }
    }
}

void reconnect()
{
    // Vòng lặp cho đến khi thiết bị được kết nối trở lại với MQTT broker.
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Tạo một ID client ngẫu nhiên để tránh xung đột khi kết nối.
        String clientId = "ESP8266_Client2";
        // clientId += String(random(0xffff), HEX); // ID được tạo dựa trên một giá trị ngẫu nhiên, giúp đảm bảo tính duy nhất.

        // Thử kết nối với MQTT broker sử dụng ID client, tên người dùng và mật khẩu.
        if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
        {
            Serial.println("connected");
            // Nếu kết nối thành công, đăng ký lại các chủ đề cần thiết.
            client.subscribe("esp8266/client1/aht20/temperature");
            client.subscribe("esp8266/client1/aht20/humidity");
        }
        else
        {
            // Nếu kết nối thất bại, in ra mã lỗi và thử lại sau 5 giây.
            Serial.print("failed, rc=");
            Serial.print(client.state()); // In ra trạng thái kết nối hiện tại để giúp xác định lỗi.
            Serial.println(" try again in 5 seconds");
            // Đợi 5 giây trước khi thử kết nối lại.
            delay(5000);
        }
    }
}

void setup()
{
    Serial.begin(9600); // Bắt đầu giao tiếp serial ở tốc độ baud rate 115200.
    max7219.Begin();
    pinMode(Btn_Green, INPUT);
    pinMode(Btn_Red, INPUT);
    setup_wifi(); // Thực hiện cấu hình kết nối WiFi.
    // Thiết lập thông tin máy chủ MQTT và gán hàm callback để xử lý tin nhắn đến.
    client.setServer(mqtt_server, mqtt_port); // Thiết lập máy chủ MQTT và cổng kết nối.
    client.setCallback(callback);             // Gán hàm callback để xử lý tin nhắn MQTT đến.
}

void loop()
{
    // Kiểm tra nếu thiết bị không kết nối với MQTT broker, thực hiện quá trình kết nối lại.
    if (!client.connected())
    {
        reconnect(); // Gọi hàm reconnect() để thực hiện kết nối lại với MQTT broker.
    }
    client.loop(); // Xử lý các gói tin MQTT đến và gửi đi trong thư viện PubSubClient.

    // Value_Red = digitalRead(Btn_Red);
    // Value_Green = digitalRead(Btn_Green);

    // // Check Button Red
    // if (Value_Red != Default_Red)
    // {
    //     if (Value_Red == 1)
    //     {
    //         Count_Red++;
    //         if (Count_Red >= 2)
    //         {
    //             Count_Red = 0;
    //         }

    //         if (Count_Red == 1)
    //         {
    //             client.publish("esp8266/client2/led/red", "On");
    //         }
    //         else
    //         {
    //             client.publish("esp8266/client2/led/red", "Off");
    //         }

    //         delay(300); // Debounce delay
    //     }
    //     Default_Red = Value_Red;
    // }

    // // Check Button Green
    // if (Value_Green != Default_Green)
    // {
    //     if (Value_Green == 1)
    //     {
    //         Count_Green++;

    //         if (Count_Green >= 2)
    //         {
    //             Count_Green = 0;
    //         }

    //         if (Count_Green == 1)
    //         {
    //             client.publish("esp8266/client2/led/green", "On");
    //         }
    //         else
    //         {
    //             client.publish("esp8266/client2/led/green", "Off");
    //         }

    //         delay(300); // Debounce delay
    //     }
    //     Default_Green = Value_Green;
    // }
}