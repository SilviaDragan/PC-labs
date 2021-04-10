#include "skel.h"

// Echipa: Silvia Dragan, Catalin Stoian, Corina Purcareanu.

int interfaces[ROUTER_NUM_INTERFACES];
struct route_table_entry *rtable;
int rtable_size;

struct arp_entry *arp_table;
int arp_table_len;

/*
 Returns a pointer (eg. &rtable[i]) to the best matching route
 for the given dest_ip. Or NULL if there is no matching route.
*/

//best route
struct route_table_entry *get_best_route(__u32 dest_ip) {
	/* TODO 1: Implement the function */
	int i, best_index = -1;
	for(i = 0; i < rtable_size; i++) {
		if( (rtable[i].mask & dest_ip) == rtable[i].prefix) {
			if(best_index == -1 || ntohl(rtable[i].mask) > ntohl(rtable[best_index].mask)) {
				best_index = i;
			}

		}
	}
	if(best_index > 0) {
		return &rtable[best_index];
	}
	else return NULL;
}

/*
 Returns a pointer (eg. &arp_table[i]) to the best matching ARP entry.
 for the given dest_ip or NULL if there is no matching entry.
*/

// structura ce contine ip si mac pt adresa la care vrem sa trimitem mesajul
struct arp_entry *get_arp_entry(__u32 ip) {
    /* TODO 2: Implement */
    int i;
    for (i=0; i < arp_table_len; i++) {
    	if(arp_table[i].ip == ip) {
    		return &arp_table[i];
    	}
    }
    return NULL;
}

int compare(const void *a, const void *b) {
	return ((struct route_table_entry *)b)->mask - ((struct route_table_entry *)a)->mask;

}

int main(int argc, char *argv[])
{
	msg m;
	int rc;

	init();
	rtable = malloc(sizeof(struct route_table_entry) * 100);
	arp_table = malloc(sizeof(struct  arp_entry) * 100);
	DIE(rtable == NULL, "memory");
	rtable_size = read_rtable(rtable);
	parse_arp_table();
	/* Students will write code here */
	qsort(rtable, rtable_size, sizeof(struct route_table_entry), compare);

	while (1) {
		rc = get_packet(&m);
		DIE(rc < 0, "get_message");
		struct ether_header *eth_hdr = (struct ether_header *)m.payload;
		struct iphdr *ip_hdr = (struct iphdr *)(m.payload + sizeof(struct ether_header));
		/* TODO 3: Check the checksum */
		if (ip_checksum(ip_hdr, sizeof(struct iphdr)) != 0) {
			printf("checksum wrong, dropping packet\n");
			continue;
		}
		/* TODO 4: Check TTL >= 1 */
		if (ip_hdr->ttl < 1) {
			printf("ttl < 1, dropping packet\n");
			continue;
		}

		/* TODO 5: Find best matching route (using the function you wrote at TODO 1) */
		struct route_table_entry *best_route = get_best_route(ip_hdr->daddr);
		if(! best_route) {
			printf("no best route\n");
			continue;
		}
		
		/* TODO 6: Update TTL and recalculate the checksum */
		ip_hdr->ttl--;
		ip_hdr->check = 0;
		ip_hdr->check = ip_checksum(ip_hdr,sizeof(struct iphdr));

		/* TODO 7: Find matching ARP entry and update Ethernet addresses */
		struct arp_entry *arp = get_arp_entry(ip_hdr->daddr);
		if(! arp) {
			printf("no arp\n");
			continue;
		}
		
		get_interface_mac(best_route->interface, eth_hdr->ether_shost);
		memcpy(eth_hdr->ether_dhost, arp->mac, 6);
	

		/* TODO 8: Forward the pachet to best_route->interface */
		send_packet(best_route->interface, &m);
	}
}
