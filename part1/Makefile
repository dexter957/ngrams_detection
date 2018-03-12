CC = gcc
CFLAG = -c
FASTFLAG1 = -O1
FASTFLAG2 = -O2
FASTFLAG3 = -O3
FASTFLAGS = -Os
FASTFLAGF = -Ofast

PROG = ngrams
SRCS = cli2.c trie_node.c binHeapAr.c trie.c main.c
HDRS = cli2.h trie_node.h binHeapAr.h trie.h
OBJS = $(SRCS: .c=.o)

$(PROG) : $(OBJS)
	$(CC) $(OBJS) -o $(PROG)

#$(PROG) : $(OBJS)
#	$(CC) $(OBJS) $(FASTFLAG1) -o $(PROG)

#$(PROG) : $(OBJS)
#	$(CC) $(OBJS) $(FASTFLAG2) -o $(PROG)

#$(PROG) : $(OBJS)
#	$(CC) $(OBJS) $(FASTFLAG3) -o $(PROG)

#$(PROG) : $(OBJS)
#	$(CC) $(OBJS) $(FASTFLAGS) -o $(PROG)

#$(PROG) : $(OBJS)
#	$(CC) $(OBJS) $(FASTFLAGF) -o $(PROG)
