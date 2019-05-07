#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <arpa/inet.h>
#include <math.h>
#include <error.h>


#define MAX_MASK_LEN 32
#define PREFIX_LEN 15   /* Max IPv4 len in A.B.C.D format */
#define SET_BIT(val, pos) (val = val | (1 << pos))
#define COMPLEMENT(x) (x = x ^ 0xFFFFFFFF)

/* Input: bit size of subnet's mask_value 
 * Return: 32 bit mask 
*/
static unsigned int get_network_mask(char mask_value)
{
    unsigned int network_mask = 0x00000000;
    int pos = 31;
    while(mask_value) {
        SET_BIT(network_mask, pos);
        --mask_value, --pos;
    }
    return network_mask;
}

static unsigned int get_ip_str_as_int(char *addr)
{
    unsigned char ipbytes[4];
    sscanf(addr, "%hhu.%hhu.%hhu.%hhu", &ipbytes[3],
            &ipbytes[2], &ipbytes[1], &ipbytes[0]);

    return ipbytes[0] | ipbytes[1] << 8 | ipbytes[2] << 16 | ipbytes[3] << 24;
}

void get_decimal_format(unsigned int ip_addr, char *output_buffer)
{
    sprintf(output_buffer, "%d.%d.%d.%d", 
        ((ip_addr & 0xFF000000) >> 24),
        ((ip_addr & 0xFF0000) >> 16),
        ((ip_addr & 0xFF00) >> 8),
        (ip_addr & 0xFF));
}

void get_network_id(char *ip_addr, char mask, char *output_buffer)
{
    unsigned int network_ip = get_ip_str_as_int(ip_addr);
    unsigned int network_mask = get_network_mask(mask);
    unsigned int network_id = (network_ip & network_mask);

    get_decimal_format(network_id, output_buffer);
}

void get_broadcast_addr(char *ip_addr, char mask, char *output_buffer)
{
    unsigned int network_ip = get_ip_str_as_int(ip_addr);
    unsigned int network_mask = get_network_mask(mask);
    unsigned int network_id = (network_ip & network_mask);

    unsigned int broadcast_addr = network_id | COMPLEMENT(network_mask);

    get_decimal_format(broadcast_addr, output_buffer);
}

unsigned int get_subnet_cardinality(char mask)
{
    unsigned int card = (pow(2, MAX_MASK_LEN - mask) - 2);
    return card;
}

int check_ip_subnet(char *network_id, char mask, char *check_ip)
{
    unsigned int id = get_ip_str_as_int(network_id);
    unsigned int ip = get_ip_str_as_int(check_ip);
    unsigned int network_mask = get_network_mask(mask);

    unsigned int check_id = ip & network_mask;

    if (check_id == id) {
        return 0;
    } else 
        return -1; 
}


int main(int argc, char **argv)
{

    /*Testing get_broadcast_addr()*/
    { 
        printf("Testing get_broadcast_addr()...\n");   

        char ip_address[PREFIX_LEN + 1], output_buffer[PREFIX_LEN + 1];
        
        memset(ip_address, 0, PREFIX_LEN + 1);
        memcpy(ip_address, "192.168.2.10", strlen("192.168.2.10"));
        ip_address[strlen(ip_address)] = '\0';
        
        char mask = 24;

        memset(output_buffer, 0 , PREFIX_LEN + 1);
        get_broadcast_addr(ip_address, mask, output_buffer);
        printf("broadcast address  = %s\n", output_buffer);
        printf("PASSED.\n");   
    }

    /*Testing get_ip_str_as_int()*/
    { 
        printf("Testing get_ip_str_as_int()...\n"); 

        char ip_address[PREFIX_LEN + 1];
        
        memset(ip_address, 0, PREFIX_LEN + 1);
        memcpy(ip_address, "192.168.2.10", strlen("192.168.2.10"));
        ip_address[strlen(ip_address)] = '\0';

        unsigned int a = get_ip_str_as_int(ip_address);
        
        printf("a = %u\n", a);
        printf("PASSED.\n");   
    }


    /*Testing get_decimal_format()*/
    {
        char output_buffer[PREFIX_LEN + 1];
        memset(output_buffer, 0 , PREFIX_LEN + 1);
        
        unsigned int a = 2058138165; 
        printf("Testing get_decimal_format()...\n");   
        
        get_decimal_format(htonl(a), output_buffer);
        printf("IP address in decimal format = %s\n", output_buffer); 
        printf("PASSED.\n");   
    }


    /*Testing get_network_id()*/
    { 
        printf("Testing get_network_id()...\n");   
        
        char ip_address[PREFIX_LEN + 1], output_buffer[PREFIX_LEN + 1];
        
        memset(ip_address, 0, PREFIX_LEN + 1);
        memcpy(ip_address, "192.168.2.10", strlen("192.168.2.10"));
        ip_address[strlen(ip_address)] = '\0';
        
        char mask = 20;
        
        memset(output_buffer, 0 , PREFIX_LEN + 1);
        get_network_id(ip_address, mask, output_buffer);
        
        printf("Network Id = %s/%u\n", output_buffer, mask);
        printf("PASSED.\n");   
    }

    /*Testing get_subnet_cardinality() */
    {
        printf("Testing get_subnet_cardinality()...\n");    
        
        char mask = 24;
        
        printf("Cardinality = %u\n", get_subnet_cardinality(mask));
        printf("PASSED.\n");
    }

    {
    /*Testing check_ip_subnet()*/    
        printf("Testing check_ip_subnet()...\n");
        
        char network_id[PREFIX_LEN + 1];
        strncpy(network_id, "192.168.1.0", strlen("192.168.1.0"));
        network_id[PREFIX_LEN] = '\0';

        char mask = 24;

        char ip_address[PREFIX_LEN + 1];
        strncpy(ip_address, "192.168.1.10",  strlen("192.168.1.10"));

        int res = check_ip_subnet(network_id, mask, ip_address);
        
        printf("IP Subnet check Result = %s\n", res == 0 ? "Membership true": "Membership false");
        printf("PASSED.\n");
    }

    return 0;
}   

