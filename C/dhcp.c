/**********************************************
			Author: Yael Lotan
			Revewer: Sandra Hadad 
**********************************************/

#include <stdlib.h> /* malloc */
#include <math.h> /* pow */
#include <assert.h> /* assert */

#include "./include/ip.h" /* ip header file */
#include "./include/trie.h" /* trie header file */
#include "./include/dhcp.h" /* dhcp header file */

struct dhcp
{
	trie_t *ip_pool;
	ip_t network_addr;
	uint32_t subnet_mask;
};

#define HEIGHT(x) (log(~x + 1) / log(2))

void DHCPDestroy(dhcp_t *dhcp)
{
	assert (NULL != dhcp);

	TrieDestroy(dhcp->ip_pool);
	free(dhcp);

	dhcp = NULL;
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
	size_t total_ip_add = 0;

	assert (NULL != dhcp);

	total_ip_add = pow(2, HEIGHT(dhcp->subnet_mask));
	
	return (total_ip_add - TrieCount(dhcp->ip_pool));
}

enum status DHCPGetAddress(dhcp_t *dhcp, ip_t ip_req, ip_t *ip_res)
{
	uint32_t req_ip = ConvertStructToNum(ip_req);
	uint32_t inserted_ip = 0;
	enum status result = 0;

	assert (NULL != dhcp);

	result = TrieInsert(dhcp->ip_pool,req_ip, &inserted_ip);

	if (NO_FREE_ADDR != result)
	{
		*ip_res = ConvertNumToStruct(inserted_ip);
	}

	return result;
}

int DHCPFreeAddr(dhcp_t *dhcp, ip_t ip_addr)
{
	int32_t key = 0;

	assert (NULL != dhcp);

	key = ConvertStructToNum(ip_addr);

	return TrieRemove(dhcp->ip_pool, key);
}

dhcp_t *DHCPCreate(ip_t network_addr, uint32_t subnet_mask)
{
	int result = 0;
	ip_t net_ip_req0 = {{0, 0, 0, 0}};
	ip_t net_ip_req1 = {{1, 1, 1, 1}};
	ip_t inserted_ip = {{0, 0, 0, 0}};

	dhcp_t *dhcp = (dhcp_t *)malloc(sizeof(*dhcp));
	if (!dhcp)
	{
		return NULL;
	}

	dhcp->ip_pool = TrieCreate(HEIGHT(subnet_mask));
	if (!dhcp->ip_pool)
	{
		free(dhcp);
		dhcp = NULL;

		return NULL;
	}

	dhcp->network_addr = network_addr;
	dhcp->subnet_mask = subnet_mask;

	result = GetAddress(dhcp, net_ip_req0, &inserted_ip);
	if (OK != result)
	{
		return NULL;
	}

	result = GetAddress(dhcp, net_ip_req1, &inserted_ip);
	if (OK != result)
	{
		return NULL;
	}

	return dhcp;
}
