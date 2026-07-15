#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

// Windows aur Linux dono ke networks ko ek sath bina kisi error ke handle karne ke liye
#ifdef _WIN32
    #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0600
    #endif
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32")
    typedef SOCKET SocketType;
    #define CLOSE_SOCKET closesocket
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    typedef int SocketType;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define CLOSE_SOCKET close
#endif

int main() {
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;
    #endif

    // Render cloud automatic variable se port uthayega, default port 10000 hai
    int port = 10000;
    if(getenv("PORT")) {
        port = std::atoi(getenv("PORT"));
    }

    SocketType server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Socket Creation Failed\n";
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port); 

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Bind Failed\n";
        CLOSE_SOCKET(server_fd);
        #ifdef _WIN32
        WSACleanup();
        #endif
        return 1;
    }

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "Listen Failed\n";
        CLOSE_SOCKET(server_fd);
        #ifdef _WIN32
        WSACleanup();
        #endif
        return 1;
    }

    std::cout << "\n=========================================\n";
    std::cout << "SERVER ACTIVE HAI: http://localhost:" << port << "\n";
    std::cout << "=========================================\n\n";

    while (true) {
        #ifdef _WIN32
        int addrlen = sizeof(address);
        #else
        socklen_t addrlen = sizeof(address);
        #endif
        
        SocketType new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        if (new_socket == INVALID_SOCKET) continue;

        char buffer[1024] = {0}; 
        #ifdef _WIN32
        recv(new_socket, buffer, 1023, 0);
        #else
        read(new_socket, buffer, 1023);
        #endif

        // Aapki original Adsterra script ke sath final premium HTML block
        std::string html_content = 
            "<!DOCTYPE html>"
            "<html lang='en'>"
            "<head>"
            "   <meta charset='UTF-8'>"
            "   <meta name='viewport' content='width=device-width, initial-scale=1.0'>"
            "   <title>Apex Digital | Premium Web Solutions</title>"
            "   <style>"
            "       * { margin: 0; padding: 0; box-sizing: border-box; font-family: sans-serif; }"
            "       body { background-color: #0a0e17; color: #ffffff; line-height: 1.6; display: flex; flex-direction: column; justify-content: center; align-items: center; min-height: 100vh; text-align: center; padding-bottom: 20px; }"
            "       .hero-content { max-width: 800px; padding: 20px; margin-bottom: 30px; }"
            "       h1 { font-size: 3.5rem; font-weight: 800; line-height: 1.2; margin-bottom: 20px; background: linear-gradient(135deg, #ffffff 30%, #00f2fe 100%); -webkit-background-clip: text; -webkit-text-fill-color: transparent; }"
            "       .hero-p { color: #a0aec0; font-size: 1.2rem; max-width: 600px; margin: 0 auto 40px auto; }"
            "       .cta-btn { display: inline-block; padding: 14px 35px; background: linear-gradient(45deg, #00f2fe, #4facfe); color: #0a0e17; font-weight: 600; text-decoration: none; border-radius: 8px; transition: all 0.3s ease; box-shadow: 0 4px 20px rgba(0, 242, 254, 0.25); }"
            "       .ad-container { width: 100%; max-width: 728px; min-height: 90px; margin-top: 25px; padding: 5px; display: flex; justify-content: center; align-items: center; background: rgba(255,255,255,0.01); border-radius: 6px; }"
            "   </style>"
            "</head>"
            "<body>"
            "   <section class='hero'>"
            "       <div class='hero-content'>"
            "           <h1>We Build High-Performance Digital Experiences</h1>"
            "           <p class='hero-p'>Engineered for speed, scale, and modern business. Transforming complex requirements into elegant web solutions.</p>"
            "           <a href='#' class='cta-btn'>Get Started</a>"
            "       </div>"
            "   </section>"
            "   <div class='ad-container'>"
            "       <script type=\"text/javascript\">"
            "           atOptions = {"
            "               'key' : '4cf7da425469f670b1ecc33f1664daec',"
            "               'format' : 'iframe',"
            "               'height' : 90,"
            "               'width' : 728,"
            "               'params' : {}"
            "           };"
            "       </script>"
            "       <script type=\"text/javascript\" src=\"https://highperformanceformat.com\"></script>"
            "   </div>"
            "</body>"
            "</html>";

        std::string response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/html; charset=UTF-8\r\n"
                               "Content-Length: " + std::to_string(html_content.length()) + "\r\n"
                               "Connection: close\r\n\r\n" + html_content;

        #ifdef _WIN32
        send(new_socket, response.c_str(), response.length(), 0);
        #else
        send(new_socket, response.c_str(), response.length(), 0);
        #endif
        
        CLOSE_SOCKET(new_socket);
    }

    CLOSE_SOCKET(server_fd);
    #ifdef _WIN32
    WSACleanup();
    #endif
    return 0;
}
