void ping(int sockfd, struct in_addr ip_addr, int count)
{
	struct packet pkt;
	init_packet(&pkt);

	struct ether_header *eth_hdr = (struct ether_header *)pkt.payload;
	struct iphdr *ip_hdr = (struct iphdr *)(pkt.payload + IP_OFF);
	struct icmphdr *icmp_hdr = (struct icmphdr *)(pkt.payload + ICMP_OFF);

	/* Init Ethernet header. */
	init_eth_hdr(sockfd, eth_hdr);

	char *ip_s = inet_ntoa(ip_addr);
	printf("PING %s ...\n", ip_s);
	printf("%d\n", count);

	/* TODO 2 implementați un ciclu astfel încât să dați ping de count ori. */
	for (int i = 0; i < count; i++) {
		/* TODO 1 Dacă vreți ca pachetul de ICMP să conțină date in
		 * payload, updatați dimensiunea totală a pachetului). */
		pkt.size = sizeof(struct ether_header) + sizeof(struct iphdr)
			+ sizeof(struct icmphdr);

		/* TODO 1 Inițializați headerul de IPv4 cu informațiile necesare.
		 * Calculați checksum. Folosiți funcția 'checksum' din utils.h
		 */

		get_interface_ip(sockfd, IFNAME, &(ip_hdr->saddr));   
		ip_hdr->daddr = ip_addr.s_addr;
		ip_hdr->ttl = 64;
		ip_hdr->version = 4;
		ip_hdr->protocol = IPPROTO_ICMP;

		ip_hdr->frag_off = 0;
		ip_hdr->tos = 0;
		ip_hdr->ihl = 5;
		ip_hdr->id = htons(1);

		ip_hdr->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
		ip_hdr->check = 0;
		ip_hdr->check = checksum(ip_hdr, sizeof(struct iphdr));

		/* TODO 1 Inițializați headerul de ICMP cu informațiile necesare.
		 * Calculați checksum. Folosiți funcția 'checksum' din utils.h
		 */
		icmp_hdr->code = 0;
		// icmp_hdr->type = ICMP_ECHO;
		icmp_hdr->type = 8;
		icmp_hdr->checksum = 0;
		icmp_hdr->un.echo.sequence = 1;
		icmp_hdr->un.echo.id = 1;
		icmp_hdr->checksum = checksum(icmp_hdr, sizeof(struct icmphdr));

		struct packet reply;
		double time_elapsed = time_packet(sockfd, &pkt, &reply);

		// normalize to miliseconds
		time_elapsed = time_elapsed * 1000;

		struct iphdr *rip_hdr = (struct iphdr *)(reply.payload + IP_OFF);
		struct icmphdr *ricmp_hdr = (struct icmphdr *)(reply.payload + ICMP_OFF);

		/* TODO 1 afișați informațiile relevante din pachetul reply */
		printf("ttl: %d \n", rip_hdr->ttl);	
	}

}