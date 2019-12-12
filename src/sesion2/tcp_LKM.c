/*****************************************************
 * This code was compiled and tested on Ubuntu 18.04.1
 * with kernel version 4.15.0
 *****************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/smp.h>

static struct nf_hook_ops *nfho = NULL;

static unsigned int hfunc(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
	{
	struct iphdr *iph;
	struct udphdr *udph;
	struct tcphdr *tcph;
	unsigned char *user_data = 0; 
    unsigned char *tail = 0;
    int datasize =0;

	if (!skb)
		return NF_ACCEPT;

	iph = ip_hdr(skb);
	switch (iph->protocol)
		{
		case IPPROTO_UDP:		/* User Datagram Protocol		*/
			udph = udp_hdr(skb);
			if (ntohs(udph->dest) == 53 || ntohs(udph->dest) == 22) 
				{
				printk(KERN_INFO "NETFILTER: UDP 53!\n");
				}
			return NF_ACCEPT;
			break;	

		case IPPROTO_TCP:		/* Transmission Control Protocol	*/
			tcph = tcp_hdr(skb);
			if (ntohs(tcph->dest) == 22 || ntohs(tcph->dest) == 445)
				return NF_ACCEPT;	
			printk(KERN_INFO "NETFILTER: Protocolo TCP CORE = %d\n", smp_processor_id());
			printk(KERN_INFO "NETFILTER: source = %u\n", ntohs(tcph->source));
			printk(KERN_INFO "NETFILTER: destino = %u\n", ntohs(tcph->dest));
			printk(KERN_INFO "NETFILTER: secuencia = %u\n", tcph->seq);
			printk(KERN_INFO "NETFILTER: ack_seq = %u\n", tcph->ack_seq);
			if (tcph->syn)
            	{
            	printk (KERN_INFO "Connect %d\r\n", iph->saddr);
            	}
        	else if (tcph->fin || tcph->rst)
            	{
            	printk (KERN_INFO "Disconnect %d\r\n", iph->saddr);
            	}
        	else
            	{
            	user_data = (unsigned char *)((unsigned char *)tcph + (tcph->doff * 4));
            	tail = skb_tail_pointer(skb);
            	if ( user_data && tail )
               		{
               		datasize = (int)((long)tail-(long)user_data);
               		if ( datasize > 0 )
                   		{
                   		printk (KERN_INFO "Datasize %d\r\n", datasize);
	                   }
    	            }
        	    }
			return NF_ACCEPT;
			break;

		case IPPROTO_IP:		/* Dummy protocol for TCP		*/
		case IPPROTO_ICMP:		/* Internet Control Message Protocol	*/
		case IPPROTO_IGMP:		/* Internet Group Management Protocol	*/
		case IPPROTO_IPIP:		/* IPIP tunnels (older KA9Q tunnels use 94) */
		case IPPROTO_EGP:		/* Exterior Gateway Protocol		*/
		case IPPROTO_PUP:		/* PUP protocol				*/
		case IPPROTO_IDP:		/* XNS IDP protocol			*/
		case IPPROTO_TP:		/* SO Transport Protocol Class 4	*/
		case IPPROTO_DCCP:		/* Datagram Congestion Control Protocol */
		case IPPROTO_IPV6:		/* IPv6-in-IPv4 tunnelling		*/
		case IPPROTO_RSVP:		/* RSVP Protocol			*/
		case IPPROTO_GRE:		/* Cisco GRE tunnels (rfc 1701,1702)	*/
		case IPPROTO_ESP:		/* Encapsulation Security Payload protocol */
		case IPPROTO_AH:		/* Authentication Header protocol	*/
		case IPPROTO_MTP:		/* Multicast Transport Protocol		*/
		case IPPROTO_BEETPH:	/* IP option pseudo header for BEET	*/
		case IPPROTO_ENCAP:		/* Encapsulation Header			*/
		case IPPROTO_PIM:		/* Protocol Independent Multicast	*/
		case IPPROTO_COMP:		/* Compression Header Protocol		*/
		case IPPROTO_SCTP:		/* Stream Control Transport Protocol	*/
		case IPPROTO_UDPLITE:	/* UDP-Lite (RFC 3828)			*/
		case IPPROTO_MPLS:		/* MPLS in IP (RFC 4023)		*/
		case IPPROTO_RAW:		/* Raw IP packets			*/
			return NF_ACCEPT;
			break;

		default:
			printk(KERN_INFO "NETFILTER: Rechazado Protocolo %d\n", iph->protocol);
			return NF_DROP;
			break;
		}
	}

static int __init LKM_init(void)
{
	nfho = (struct nf_hook_ops*)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);
	
	/* Initialize netfilter hook */

	nfho->hook 	= (nf_hookfn*)hfunc;			/* hook function */
	nfho->hooknum 	= NF_INET_PRE_ROUTING;		/* received packets */
	nfho->pf 	= PF_INET;						/* IPv4 */
	nfho->priority 	= NF_IP_PRI_FIRST;			/* max hook priority */
	
	nf_register_net_hook(&init_net, nfho);
	printk(KERN_INFO "TCP: Welcome!\n");
	return 0;
}

static void __exit LKM_exit(void)
{
	nf_unregister_net_hook(&init_net, nfho);
	printk(KERN_INFO "TCP: By!\n");
	kfree(nfho);
}

module_init(LKM_init);
module_exit(LKM_exit);