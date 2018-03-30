CC = gcc
CFLAG = -c
VPATH = ./linear_hash:./trie_nodes
LIB = -lm
OFLAG = -o
FASTFLAG1 = -O1
FASTFLAG2 = -O2
FASTFLAG3 = -O3
FASTFLAGS = -Os
FASTFLAGF = -Ofast
PROFILER  = -pg
PROG = ngrams
HRDS = trie.h trie_node.h murmurHash.h bloomFilter.h semistatic_trie_node_v2.h cli2.h treap.h trie_node_connector.h linear_hash.h
SRCS = main.c trie.c trie_node.c murmurHash.c bloomFilter.c semistatic_trie_node_v2.c cli2.c treap.c trie_node_connector.c linear_hash_v2.c 

BLOBJS = ./bloomFilter.o
TRIEOBJS = ./trie.o
CLIOBJS = ./cli2.o
NODEOBJS = ./trie_nodes/semistatic_trie_node_v2.o
TREOBJS = ./treap.o
MUROBJS = ./murmurHash.o
HASHOBJS = ./linear_hash/linear_hash_v3.o
CONNOBJS = ./trie_nodes/trie_node_connector.o
MAINOBJS = ./main.o
STTRIOBJS = ./trie_nodes/static_trie_node_semi.o

$(PROG) : $(MAINOBJS) $(BLOBJS) $(TRIEOBJS) $(CLIOBJS) $(NODEOBJS) $(TREOBJS) $(MUROBJS) $(HASHOBJS) $(CONNOBJS) $(STTRIOBJS)
	$(CC) $(OFLAG) $@ $^ $(LIB)

#Compilation rules for optimization flags
#$(PROG) : $(MAINOBJS) $(BLOBJS) $(TRIEOBJS) $(CLIOBJS) $(NODEOBJS) $(TREOBJS) $(MUROBJS) $(HASHOBJS) $(CONNOBJS) $(STTRIOBJS)
#	$(CC) $(FASTFLAG1) $(OFLAG) $@ $^ $(LIB)

#$(PROG) : $(MAINOBJS) $(BLOBJS) $(TRIEOBJS) $(CLIOBJS) $(NODEOBJS) $(TREOBJS) $(MUROBJS) $(HASHOBJS) $(CONNOBJS) $(STTRIOBJS)
#	$(CC) $(FASTFLAG2) $(OFLAG) $@ $^ $(LIB)

#$(PROG) : $(MAINOBJS) $(BLOBJS) $(TRIEOBJS) $(CLIOBJS) $(NODEOBJS) $(TREOBJS) $(MUROBJS) $(HASHOBJS) $(CONNOBJS) $(STTRIOBJS)
#	$(CC) $(FASTFLAG3) $(OFLAG) $@ $^ $(LIB)

#$(PROG) : $(MAINOBJS) $(BLOBJS) $(TRIEOBJS) $(CLIOBJS) $(NODEOBJS) $(TREOBJS) $(MUROBJS) $(HASHOBJS) $(CONNOBJS) $(STTRIOBJS)
#	$(CC) $(FASTFLAGS) $(OFLAG) $@ $^ $(LIB)

#$(PROG) : $(MAINOBJS) $(BLOBJS) $(TRIEOBJS) $(CLIOBJS) $(NODEOBJS) $(TREOBJS) $(MUROBJS) $(HASHOBJS) $(CONNOBJS) $(STTRIOBJS)
#	$(CC) $(FASTFLAGF) $(OFLAG) $@ $^ $(LIB)

#Compilation rule for profiler
#$(PROG) : $(MAINOBJS) $(BLOBJS) $(TRIEOBJS) $(CLIOBJS) $(NODEOBJS) $(TREOBJS) $(MUROBJS) $(HASHOBJS) $(CONNOBJS) $(STTRIOBJS)
#	$(CC) $(PROFILER) $(OFLAG) $@ $^ $(LIB)

#clean
.PHONY : clean

#Recipes

#Bloom Filter
$(BLOBJS): bloomFilter.c bloomFilter.c murmurHash.h
	$(CC) $(CFLAG) $< -o $@

#Murmur
$(MUROBJS): murmurHash.c murmurHash.h
	$(CC) $(CFLAG) $< -o $@

#cli
$(CLIOBJS): cli2.c cli2.h trie.h
	$(CC) $(CFLAG) $< -o $@

#Treap
$(TREOBJS): treap.c treap.h 
	$(CC) $(CFLAG) $< -o $@

#Trie
$(TRIEOBJS): trie.c trie.h bloomFilter.h treap.h semistatic_trie_node_v2.h
	$(CC) $(CFLAG) $< -o $@

#static nodes
#$(STTRIOBJS): ./trie_nodes/static_trie_node_semi_improved.c ./trie_nodes/static_trie_node_semi_improved.h
#	$(CC) $(CFLAG) $< -o $@

$(STTRIOBJS): ./trie_nodes/static_trie_node_semi.c ./trie_nodes/static_trie_node_semi.h
	$(CC) $(CFLAG) $< -o $@

#trie_node
#$(NODEOBJS): trie_node.c trie_node.h
#	$(CC) $(CFLAG) $< -o $@

#semistatic_trie_node_v2
$(NODEOBJS) : semistatic_trie_node_v2.c semistatic_trie_node_v2.h
	$(CC) $(CFLAG) $< -o $@

#semistatic_trie_node
#$(NODEOBJS): semistatic_trie_node.c semistatic_trie_node.h
#	$(CC) $(CFLAG) $< -o $@

#linear_hash
$(HASHOBJS): linear_hash_v3.c linear_hash_v3.h trie_node_connector.h
	$(CC) $(CFLAG) $< -o $@

#(HASHOBJS): linear_hash_v2.c linear_hash_v2.h trie_node_connector.h
#	$(CC) $(CFLAG) $< -o $@

#connector
$(CONNOBJS): trie_node_connector.c trie_node_connector.h static_trie_node_semi.h
	$(CC) $(CFLAG) $< -o $@

#main
./main.o : main.c
	$(CC) $(CFLAG) $< -o $@

#clean
clean:
	rm $(MAINOBJS) $(BLOBJS) $(TRIEOBJS) $(CLIOBJS) $(NODEOBJS) $(TREOBJS) $(MUROBJS) $(HASHOBJS) $(CONNOBJS) $(STTRIOBJS)


