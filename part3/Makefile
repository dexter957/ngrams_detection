CC = gcc
CFLAG = -c
VPATH = ./linear_hash:./trie_nodes
LIB = -lm
THREADS = -pthread
OFLAG = -o
FASTFLAG1 = -O1
FASTFLAG2 = -O2
FASTFLAG3 = -O3
FASTFLAGS = -Os
FASTFLAGF = -Ofast
PROFILER  = -pg
PROG = ngrams
HDRS = trie.h scheduler.h cli2.h treap.h bloomFilter.h murmurHash.h linear_hash_v2.h linear_hash_v3.h trie_node.h semistatic_trie_node.h semistatic_trie_node_v2.h trie_node_connector.h static_trie_node_semi.h static_trie_node_semi_improved.h tokenizers.h specs.h
SRCS = main.c trie.c scheduler.c cli2.c treap.c bloomFilter.c murmurHash.c linear_hash_v2.c linear_hash_v3.c trie_node.c semistatic_trie_node.c semistatic_trie_node_v2.c static_trie_node_semi.c static_trie_node_semi_improved.c tokenizers.c
OBJS = $(SRCS: .c=.o)

$(PROG) : $(OBJS)
	$(CC) $(OFLAG) $@ $^ $(LIB) $(THREADS)


#Compilation rules for optimization flags
#$(PROG) : $(OBJS)
#	$(CC) $(FASTFLAG1) $(OFLAG) $@ $^ $(LIB) $(THREADS)

#$(PROG) : $(OBJS)
#	$(CC) $(FASTFLAG2) $(OFLAG) $@ $^ $(LIB) $(THREADS)

#$(PROG) : $(OBJS)
#	$(CC) $(FASTFLAG3) $(OFLAG) $@ $^ $(LIB) $(THREADS)

#$(PROG) : $(OBJS)
#	$(CC) $(FASTFLAGS) $(OFLAG) $@ $^ $(LIB) $(THREADS)

#$(PROG) : $(OBJS)
#	$(CC) $(FASTFLAGF) $(OFLAG) $@ $^ $(LIB) $(THREADS)

.PHONY : clean
clean : 
	rm $(PROG)


