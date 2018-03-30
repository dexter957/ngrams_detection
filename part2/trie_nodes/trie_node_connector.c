#include "trie_node_connector.h"



typedef struct connector{
	int isStatic;

	//Dynamic connecting function pointers
	funcDynamicAllocateTrie_Node 			dAllocateTrie_Node;
	funcDynamicInitialiseTrie_Node 			dInitialiseTrie_Node;
	funcDynamicGetChildrenPtr 				dGetChildrenPtr;
	funcDynamicGetActualNumOfChildrenPtr 	dGetActualNumOfChildrenPtr;
	funcDynamicGetCertainChildPtr 			dGetCertainChildPtr;
	funcDynamicDestroyNode 					dDestroyNode;
	funcDynamicTraverseAndDestroy 			dTraverseAndDestroy;
	funcDynamicDeleteANode 					dDeleteANode;
	funcDynamicSetNumOfChildrenPtr 			dSetNumOfChildrenPtr;
	funcDynamicSetChildrenPtr 				dSetChildrenPtr;
	funcDynamicInsertAChild 				dInsertAChild;
	funcDynamicSetIs_FinalPtr 				dSetIs_FinalPtr;
	funcDynamicSearch 				 		dSearch;
	funcDynamicGetNumOfChildrenPtr 			dGetNumOfChildrenPtr;
	funcDynamicGetIs_FinalPtr 				dGetIs_FinalPtr;
	funcDynamicGetWordPtr 					dGetWordPtr;
	funcDynamicSetWordPtr 					dSetWordPtr;
	funcDynamicSetActualNumOfChildrenPtr 	dSetActualNumOfChildrenPtr;
	funcDynamicRepointToWords 				dRepointToWords;

	//Static connecting function pointers
	funcStaticAllocateTrie_Node 			sAllocateTrie_Node;
	funcStaticInitialiseTrie_Node 			sInitialiseTrie_Node;
	funcStaticGetChildrenPtr 				sGetChildrenPtr;
	funcStaticGetActualNumOfChildrenPtr 	sGetActualNumOfChildrenPtr;
	funcStaticGetCertainChildPtr 			sGetCertainChildPtr;
	funcStaticDestroyNode 					sDestroyNode;
	funcStaticTraverseAndDestroy 			sTraverseAndDestroy;
	funcStaticDeleteANode 					sDeleteANode;
	funcStaticSetNumOfChildrenPtr 			sSetNumOfChildrenPtr;
	funcStaticSetChildrenPtr 				sSetChildrenPtr;
	funcStaticInsertAChild 					sInsertAChild;
	funcStaticSetIs_FinalPtr 				sSetIs_FinalPtr;
	funcStaticSearch 						sSearch;
	funcStaticGetNumOfChildrenPtr 			sGetNumOfChildrenPtr;
	funcStaticGetIs_FinalPtr 				sGetIs_FinalPtr;
	funcStaticGetIs_FinalPositionPtr 		sGetIs_FinalPositionPtr;
	funcStaticGetWordPtr 					sGetWordPtr;
	funcStaticGetWordPositionPtr 			sGetWordPositionPtr;
	funcStaticSetWordPtr 					sSetWordPtr;
	funcStaticSetActualNumOfChildrenPtr 	sSetActualNumOfChildrenPtr;
	funcStaticRepointToWords 				sRepointToWords;

}connector;



connectorPtr createConnector(){
	connectorPtr conn;

	conn = malloc(sizeof(connector));

	conn->isStatic = -1;

	conn->dAllocateTrie_Node = NULL;
	conn->dInitialiseTrie_Node = NULL;
	conn->dGetChildrenPtr = NULL;
	conn->dGetActualNumOfChildrenPtr = NULL;
	conn->dGetCertainChildPtr = NULL;
	conn->dDestroyNode = NULL;
	conn->dTraverseAndDestroy = NULL;
	conn->dDeleteANode = NULL;
	conn->dSetNumOfChildrenPtr = NULL;
	conn->dSetChildrenPtr = NULL;
	conn->dInsertAChild = NULL;
	conn->dSetIs_FinalPtr = NULL;
	conn->dSearch = NULL;
	conn->dGetNumOfChildrenPtr = NULL;
	conn->dGetIs_FinalPtr = NULL;
	conn->dGetWordPtr = NULL;
	conn->dSetWordPtr = NULL;
	conn->dSetActualNumOfChildrenPtr = NULL;
	conn->dRepointToWords = NULL;

	conn->sAllocateTrie_Node = NULL;
	conn->sInitialiseTrie_Node = NULL;
	conn->sGetChildrenPtr = NULL;
	conn->sGetActualNumOfChildrenPtr = NULL;
	conn->sGetCertainChildPtr = NULL;
	conn->sDestroyNode = NULL;
	conn->sTraverseAndDestroy = NULL;
	conn->sDeleteANode = NULL;
	conn->sSetNumOfChildrenPtr = NULL;
	conn->sSetChildrenPtr = NULL;
	conn->sInsertAChild = NULL;
	conn->sSetIs_FinalPtr = NULL;
	conn->sSearch = NULL;
	conn->sGetNumOfChildrenPtr = NULL;
	conn->sGetIs_FinalPtr = NULL;
	conn->sGetIs_FinalPositionPtr = NULL;
	conn->sGetWordPtr = NULL;
	conn->sGetWordPositionPtr = NULL;
	conn->sSetWordPtr = NULL;
	conn->sSetActualNumOfChildrenPtr = NULL;
	conn->sRepointToWords = NULL;

	return conn;
}


void setConnector(connectorPtr conn, int type){
	conn->isStatic = type;

	if(type == DYNAMIC){
		conn->dAllocateTrie_Node = &allocateTrie_Node;
		conn->dInitialiseTrie_Node = &initialiseTrie_Node;
		conn->dGetChildrenPtr = &getChildrenPtr;
		conn->dGetActualNumOfChildrenPtr = &getActualNumOfChildrenPtr;
		conn->dGetCertainChildPtr = &getCertainChildPtr;
		conn->dDestroyNode = &destroyNode;
		conn->dTraverseAndDestroy = &traverseAndDestroy;
		conn->dDeleteANode = &deleteANode;
		conn->dSetNumOfChildrenPtr = &setNumOfChildrenPtr;
		conn->dSetChildrenPtr = &setChildrenPtr;
		conn->dInsertAChild = &insertAChild;
		conn->dSetIs_FinalPtr = &setIs_FinalPtr;
		conn->dSearch = &searchPositionBinary;
		conn->dGetNumOfChildrenPtr = &getNumOfChildrenPtr;
		conn->dGetIs_FinalPtr = &getIs_FinalPtr;
		conn->dGetWordPtr = &getWordPtr;
		conn->dSetWordPtr = &setWordPtr;
		conn->dSetActualNumOfChildrenPtr = &setActualNumOfChildrenPtr;
		conn->dRepointToWords = &repointToWords;
	}

	if(type == STATIC){
		conn->sAllocateTrie_Node = &static_allocateTrie_Node;
		conn->sInitialiseTrie_Node = &static_initialiseTrie_Node;
		conn->sGetChildrenPtr = &static_getChildrenPtr;
		conn->sGetActualNumOfChildrenPtr = &static_getActualNumOfChildrenPtr;
		conn->sGetCertainChildPtr = &static_getCertainChildPtr;
		conn->sDestroyNode = &static_destroyNode;
		conn->sTraverseAndDestroy = &static_traverseAndDestroy;
		conn->sDeleteANode = &static_deleteANode;
		conn->sSetNumOfChildrenPtr = &static_setNumOfChildrenPtr;
		conn->sSetChildrenPtr = &static_setChildrenPtr;
		conn->sInsertAChild = &static_insertAChild;
		conn->sSetIs_FinalPtr = &static_setIs_FinalPtr;
		conn->sSearch = &searchStatic;
		conn->sGetNumOfChildrenPtr = &static_getNumOfChildrenPtr;
		conn->sGetIs_FinalPtr = &static_getIs_FinalPtr;
		conn->sGetIs_FinalPositionPtr = &static_getIs_FinalPosition;
		conn->sGetWordPtr = &static_getWordPtr;
		conn->sGetWordPositionPtr = &static_getWordPositionPtr;
		conn->sSetWordPtr = &static_setWordPtr;
		conn->sSetActualNumOfChildrenPtr = &static_setActualNumOfChildrenPtr;
		conn->sRepointToWords = &static_repointToWords;
	}
}


void deleteConnector(connectorPtr conn){
	free(conn);
}


int isStatic(connectorPtr conn){
	return conn->isStatic;
}




funcAllocateTrie_Node AllocateTrie_Node(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dAllocateTrie_Node;
	if(conn->isStatic == STATIC) return conn->sAllocateTrie_Node;
}


funcInitialiseTrie_Node InitialiseTrie_Node(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dInitialiseTrie_Node;
	if(conn->isStatic == STATIC) return conn->sInitialiseTrie_Node;
}


funcDestroyNode DestroyNode(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dDestroyNode;
	if(conn->isStatic == STATIC) return conn->sDestroyNode;
}


funcTraverseAndDestroy TraverseAndDestroy(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dTraverseAndDestroy;
	if(conn->isStatic == STATIC) return conn->sTraverseAndDestroy;
}


funcDeleteANode DeleteANode(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dDeleteANode;
	if(conn->isStatic == STATIC) return conn->sDeleteANode;
}


funcInsertAChild InsertAChild(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dInsertAChild;
	if(conn->isStatic == STATIC) return conn->sInsertAChild;	
}


funcRepointToWords RepointToWords(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dRepointToWords;
	if(conn->isStatic == STATIC) return conn->sRepointToWords;
}




funcSetWordPtr SetWordPtr(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dSetWordPtr;
	if(conn->isStatic == STATIC) return conn->sSetWordPtr;	
}


funcSetNumOfChildrenPtr SetNumOfChildrenPtr(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dSetNumOfChildrenPtr;
	if(conn->isStatic == STATIC) return conn->sSetNumOfChildrenPtr;
}


funcSetChildrenPtr SetChildrenPtr(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dSetChildrenPtr;
	if(conn->isStatic == STATIC) return conn->sSetChildrenPtr;
}


funcSetIs_FinalPtr SetIs_FinalPtr(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dSetIs_FinalPtr;
	if(conn->isStatic == STATIC) return conn->sSetIs_FinalPtr;	
}


funcSetActualNumOfChildrenPtr SetActualNumOfChildrenPtr(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dSetActualNumOfChildrenPtr;
	if(conn->isStatic == STATIC) return conn->sSetActualNumOfChildrenPtr;
}




funcGetChildrenPtr GetChildrenPtr(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dGetChildrenPtr;
	if(conn->isStatic == STATIC) return conn->sGetChildrenPtr;
}


funcGetActualNumOfChildrenPtr GetActualNumOfChildrenPtr(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dGetActualNumOfChildrenPtr;
	if(conn->isStatic == STATIC) return conn->sGetActualNumOfChildrenPtr;
}


funcGetCertainChildPtr GetCertainChildPtr(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dGetCertainChildPtr;
	if(conn->isStatic == STATIC) return conn->sGetCertainChildPtr;
}

funcGetNumOfChildrenPtr GetNumOfChildrenPtr(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dGetNumOfChildrenPtr;
	if(conn->isStatic == STATIC) return conn->sGetNumOfChildrenPtr;	
}


funcGetIs_FinalPtr GetIs_FinalPtr(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dGetIs_FinalPtr;
	if(conn->isStatic == STATIC) return conn->sGetIs_FinalPtr;	
}


funcGetWordPtr GetWordPtr(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return conn->dGetWordPtr;
	if(conn->isStatic == STATIC) return conn->sGetWordPtr;	
}




size_t GetSizeOf(connectorPtr conn){
	if(conn->isStatic == DYNAMIC) return getSizeOf();
	if(conn->isStatic == STATIC) return static_getSizeOf();
}




funcDynamicSearch DynamicSearch(connectorPtr conn){
	return conn->dSearch;
}


funcStaticSearch StaticSearch(connectorPtr conn){
	return conn->sSearch;
}


funcStaticGetIs_FinalPositionPtr GetIs_FinalPositionPtr(connectorPtr conn){
	return conn->sGetIs_FinalPositionPtr;
}


funcStaticGetWordPositionPtr GetWordPositionPtr(connectorPtr conn){
	return conn->sGetWordPositionPtr;
}