#include <iostream>
#include <WinSock2.h>
#include <string>
#include <vector>
#include <fstream>

#if defined(_MSC_VER)
#pragma warning(pop)
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifdef __GNUC__
#pragma GCC visibility pop
#endif

#define _CRT_SECURE_NO_WARNINGS
constexpr auto Max_sing = 200;
#pragma comment(lib, "ws2_32.lib")

// private_key : SNBAe3IDL7yzVLf8_
// api:https://api.seniverse.com/v3/weather/now.json?key=SNBAe3IDL7yzVLf8_&location=beijing&language=zh-Hans&unit=c

/// <summary>
/// 查询天气的数据体
/// </summary>
struct Universe_data
{
private:
    const std::string HostServer = "api.seniverse.com";  //主机域名
    const std::string Private_Key = "SNBAe3IDL7yzVLf8_"; // aip密钥
    std::string Location_Cty;                            //需要查询的城市
    const std::string Language = "zh-Hans";              //显示的语言

public:
    Universe_data(const std::string &city)
    {
        this->Location_Cty = city;
    }

    //对请求进行封装
    std::string URL_string()
    {
        return "GET /v3/weather/now.json?key=" +
               this->Private_Key + "&location=" +
               this->Location_Cty + "&language=" +
               this->Language + "&unit=c\n\r\n\r";
    }
    /// <summary>
    /// 返回主机域名与密钥
    /// </summary>
    /// <returns></returns>
    std::string Rtu_Host() const
    {
        return this->HostServer;
    }
    std::string Rtu_Pve_key() const
    {
        return this->Private_Key;
    }
};

struct Socket_Like
{
private:
    std::string Host_name;
    unsigned short Port_name;
    char Data[Max_sing] = {0};

public:
    Socket_Like(const std::string &host, unsigned short port)
    {
        this->Host_name = host;
        this->Port_name = port;
    }
    //返回服务器名称
    std::string Rru_Host_name() const
    {
        return this->Host_name;
    }
    //初始化套接字
    void Socket_strata()
    {
        WSADATA data;

        if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
        {
            std::cout << "WSAStartup failed" << WSAGetLastError() << std::endl;
            return;
        }
    }

    SOCKET connectToHost()
    {
        //创建套接字
        SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (fd == INVALID_SOCKET)
        {
            std::cout << "socket failed" << WSAGetLastError() << std::endl;
            return INVALID_SOCKET;
        }

        SOCKADDR_IN addr;                       //创建套接字
        addr.sin_family = AF_INET;              //使用地址族，也就是ipv4/ipv6
        addr.sin_port = htons(this->Port_name); //访问端口
                                                // addr.sin_addr.S_un.S_addr = inet_addr(dynamic_cast<const char *>()));     //这里填写主机ip地址
                                                //将地址进行解析
        HOSTENT *hostent = gethostbyname(this->Host_name.c_str());
        if (hostent)
        {
            // std::cout << hostname->h_name << std::endl;
            // std::cout << hostent->h_addr_list[0] << std::endl;
            //将进行转换后的dns的ip地址复制到addr.sin_addr
            memcpy(&addr.sin_addr, hostent->h_addr_list[0], sizeof(addr.sin_addr));
        }
        else
        {
            std::cout << "解析失败" << std::endl;
            return INVALID_SOCKET;
        }

        //判断是否连接成功
        if (0 != connect(fd, (const sockaddr *)&addr, sizeof(addr)))
        {
            std::cout << "connect failed" << WSAGetLastError() << std::endl;
            return INVALID_SOCKET;
        }
        return fd;
    }

    void
    Opensocket_sent(SOCKET start, const char *url, int mension)
    {

        if (0 > send(start, url, strlen(url), mension))
        {
            std::cout << "发送失败" << WSAGetLastError() << std::endl;
        }
        else
        {
            if (0 >= recv(start, Data, Max_sing, 0))
            {
                std::cout << "recv failed" << WSAGetLastError() << std::endl;
            }
            else
            {
                puts(Data);
            }
        }
    }
    //

    void Socket_quit(SOCKET ID_name)
    {
        closesocket(ID_name);
    }

    std::string Rru_Data() const
    {
        std::string pasinpo;
        for (int i = 0; i < Max_sing; i++)
        {
            if (Data[i] == '/0')
            {
                break;
            }
            pasinpo.push_back(Data[i]);
        }
        return pasinpo;
    }
};

int main()
{
    std::fstream File_json;
    Universe_data *Uniats = new Universe_data("beijing");
    Socket_Like *socket_on = new Socket_Like(Uniats->Rtu_Host(), 80);
    socket_on->Socket_strata();
    //初始化运行socket
    SOCKET ST = socket_on->connectToHost();

    if (ST != INVALID_SOCKET)
    {
        std::cout << "socket like right OK >>连接成功  状态:" << WSAGetLastError() << "码" << std::endl;
    }

    socket_on->Opensocket_sent(ST, Uniats->URL_string().c_str(), 0);
    socket_on->Socket_quit(ST);

    File_json.open("../../cbete/data.json", std::ios::in | std::ios::out);
    if (!File_json)
    {
        std::cout << "erreo"
                  << "无法创建文件" << std::endl;
    }

    File_json << socket_on->Rru_Data();

    File_json.close();
    system("pause");

    return 0;
}