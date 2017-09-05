#include "ipaddress.h"
#include <sstream>
std::string IPAddress::Get(void) {

    std::ostringstream ret;

    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ - 1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    ret << "wlan: " << inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    ret << "              eth: " << inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

    close(fd);

    return ret.str();
}
