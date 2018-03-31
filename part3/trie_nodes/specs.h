#ifndef _SPECSH_
#define _SPECSH_


//#define DYN_NODE 1 		//trie_node
//#define DYN_NODE 2	//semistatic_trie_node
#define DYN_NODE 3	//semistatic_trie_node_v2

//#define STAT_NODE 1		//static_trie_node_semi
#define STAT_NODE 2	//static_trie_node_semi_improved

//#define LIN_HASH 1		//linear_hash_v2
#define LIN_HASH 2		//linear_hash_v3

#define COMP_DYN 0		//to not compress dynamic trie after each F instruction
//#define COMP_DYN 1	//to compress dynamic trie sfter each F instruction with threads

#endif