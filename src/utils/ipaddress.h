#include <stdio.h>
#include <string.h> /* for strncpy */
#include <unistd.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

class IPAddress {

  public:
    static std::string Get(void);
};
