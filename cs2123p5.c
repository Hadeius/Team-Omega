/**********************************************************************
 cs2123p5.c by TEAM OMEGA [Farhad Jijina, Lane Burris, Joshua Weigand]
 
 Purpose:
 
 Command Parameters:
 
 Input:
 
 Results:
 
 Returns:
 
 Notes:
 
 **********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p5.h"


/**************************************************************************
                            Helper Functions
 ***************************************************************************/

/**************************** prettyPrintMenu *****************************
 void prettyPrintMenu(NodeT *p, int iIndent)
 Purpose:
    Traverses the tree, prints out the siblings, and their details in the
 form of a price menu.
 Parameters:
 I      NodeT *p        Pointer directed to the root of the Tree
 I      int Indent       Used to determine starting point of indented print
 Returns:
 N/A
 **************************************************************************/
void prettyPrintMenu(NodeT *p, int iIndent)
{
    //Local Variables
    int i;
    NodeT *pTitle = p->element.szTitle;
    NodeT *pCost = p->element.dCost;
    NodeT *pCostIdn = p->element.cCostInd;

    if (p == NULL)
        return;
    for (i = 0; i < iIndent; i++)
      printf("   ");
      if(pCostIdn == 0) //Prints the szTitle of each node NOT containing a price
         printf("  %2s\n", *pTitle);
      else              //Prints the szTitle of each node containing a price
      {
        printf("  %2s", *pTitle);
        printf("\t %d", *pCost); //Still Needs Work (Allignment)
      }

    prettyPrintT(p->pChild,iIndent+1);
    prettyPrintT(p->pSibling,iIndent+1);
}









/**************************************************************************
            Main Functions (NOTE: Helper Functions Listed Above)
 ***************************************************************************/

/******************************* findId ***********************************
 NodeT *findId(NodeT *p, char szId[])
 Purpose:
    Recursively going through the tree to find the specific Id we are looking for.
 Parameters:
    NodeT *p        Node that is being Id searched.
    char szId[]     Character array containing the Id we are searching
                        for.
 Returns:
    Returns the child we are looking that has the specified Id.
 **************************************************************************/
NodeT *findId(NodeT *p, char szId[])
{
    NodeT *pFound = NULL;
    //base case
    if (p == NULL)
        return NULL;

    //szId is found
    if (strcmp(p->element.szId,szId)==0)
        return p;

    //iterate through the rest of the tree
    if (p->pSibling != NULL)
        pFound = findId(p->pSibling, szId);
    if (pFound != NULL)
        return pFound;
    if (p->pChild != NULL)
        pFound =findId(p->pChild, szId);

    return pFound;
}

/***************************** findParent *********************************
 NodeT *findParent(NodeT *pParent, NodeT *p, NodeT *pkid)
 Purpose:
    Finds the parent of the child we are currently on.
 Parameters:
    NodeT *pParent
    NodeT *p
    NodeT *pkid
 Returns:
    Returns that specified child's parent node.
 **************************************************************************/
NodeT *findParent(NodeT *pParent, NodeT *p, NodeT *pkid)
{
    NodeT *pFound = NULL;
    if (p == NULL)
        return NULL;
    if (strcmp(p->element.szId,pkid->element.szId)==0)
        return pParent;
    if (p->pSibling !=NULL)
        pFound = findParent(pParent,p->pSibling,pkid);
    if (pFound != NULL)
        return pFound;
    if (p->pChild != NULL)
        pFound = findParent(p,p->pChild,pkid);
    return pFound;
}

/*************************** printPriceMenu *******************************
 void printPriceMenu(Tree tree)
 Purpose:
 
 Parameters:
 
 Returns:
 
 **************************************************************************/
void printPriceMenu(Tree tree)
{
    //Local Variable(s)
    Node *pCurrentNode = tree->pRoot;

    //Price Menu Title
    printf("Price Menu\n");

    //Traverse the tree and print each nodes Title
    prettyPrintMenu(*pCurrentNode, 1); //Start indents = 1
}

/****************************** printOne **********************************
 void printOne(Tree tree, char szId[])
 Purpose:
    Supposed to print only one part of the tree
        - Title
        - Cost
 Parameters:
 I      Tree tree       Struct containing the Root and Nodes of a tree
 IO     char szId[]     Element Identification
 Returns:
 
**************************************************************************/
void printOne(Tree tree, char szId[])
{
    NodeT *p = findId(tree->pRoot,szId);
    if (p == NULL)
        printf("PRINT ERROR: Id %s not found\n", szId);
    else
        printf("PRINT ONE:\n Title: %s Cost: %lf\n"
                ,p->element.szTitle
                ,p->element.dCost);
}
/****************************** freeSubTree *******************************
 void freeSubTree(NodeT *p)
 Purpose:
    Frees the current node and its children.
 Parameters:
    NodeT *p    Node that will be freed.
 Returns:
 
 **************************************************************************
void freeSubTree(NodeT *p)
{

    if (p == NULL)
    {
        return;
    }
    
    if (p->pChild == NULL && p->pSibling == NULL)
    {
        free(p);
        return ;
    }
    
    else if (p->pSibling != NULL)
    {
        freeTree(p->pSibling);
        freeTree(p->pChild);
        free(p);
    }
    
    else
    {
        freeTree(p->pChild);
        free(p);
    }
    
}

********************************* freeTree *******************************
 void freeTree(Tree tree)
 Purpose:
    Frees the whole tree.
 Parameters:
    I Tree tree     Tree that is being passed in for freeing.
 Returns:
 **************************************************************************
void freeTree(Tree tree)
{
    NodeT *p = tree->pRoot;
    
    if (p == NULL)
    {
        return;
    }
    
    if (p->pChild == NULL && p->pSibling == NULL)
    {
        free(p);
        return ;
    }
    
    else if (p->pSibling != NULL)
    {
        freeTree(p->pSibling);
        freeTree(p->pChild);
        free(p);
    }
    
    else
    {
        freeTree(p->pChild);
        free(p);
    }
    
}

************************** insertPriceMenu *******************************
 void insertPriceMenu(Tree tree, Element element, char szParentId[])
 Purpose:
 
 Parameters:
 
 Returns:
 
 **************************************************************************/
void insertPriceMenu(Tree tree, Element element, char szParentId[])
{
    
    
}

/************************** determineQuote *******************************
 QuoteResult determineQuote(Tree tree, QuoteSelection quoteSelection)
 Purpose:
    . After the QUOTE END command is received, this is called.
    . Prints the quote details including the title and cost for each option value.
    . Handles error cases and understands a partial quote.
 Parameters:
    I Tree tree                         Tree being passed in to be traversered
                                            through for the quote
 
    I/O QuoteSelection quoteSelection
 Returns:
    Returns a QuoteResult which includes a total cost
 **************************************************************************/
QuoteResult determineQuote(Tree tree, QuoteSelection quoteSelection)
{
    int i = 0;
    QuoteResult quote;
    while (i <= quoteSelection->iQuoteItemCnt)
    {
        NodeT *p = findId(tree->pRoot,quoteSelection->quoteItemM[i].szOptionId);
        switch(quoteSelection->quoteItemM[i].iLevel)
        {

        }

    }
    // QuoteResult is supposed to be dTotalCost;
    return quote; //only put here to prevent error messages
}

/********************************* deleteItem *******************************
 void deleteItem(Tree tree, char szId[])
 Purpose:
    Uses freeSubTree to free current node as well as the children of the current node.
    After freeSubTree is used, reconnects that sub-tree's sibiling to the parent, or to the other sub-tree's sibiling.
 Parameters:
    I/O Tree tree
 Returns:
 
 **************************************************************************/
void deleteItem(Tree tree, char szId[])
{
    
    //freeSubTree(
    
}

/**************************************************************************
    Driver function to process a input data line (Include in Driver?)
 ***************************************************************************/

/********************************* processCommand *******************************
 void processCommand(Tree tree, QuoteSelection quoteSelection, char *pszInput)
 
 Purpose:
 
 Parameters:
 
 Returns:
 
 **************************************************************************/

void processCommand(Tree tree, QuoteSelection quoteSelection, char *pszInput)
{
    char szToken[MAX_TOKEN_SIZE+1];
    char szSubordinateToId[MAX_ID_SIZE];
    char szOptionId[MAX_ID_SIZE];
    NodeT *p;
    Element element;
    
    
    //Gets first word in input
    pszInput = getToken(pszInput,szToken,MAX_TOKEN_SIZE);
    
    //Set of if statmenets to check the command
    if (strcmp(szToken,"DEFINE")==0)
    {
        //checks the next word after the command
        pszInput = getToken(pszInput,szToken,MAX_TOKEN_SIZE);
        
        //check to see if it is OPTION or VALUE
        
        if (strcmp(szToken,"OPTION")==0)
        {
            sscanf(pszInput, "%s %s %[^\t\n]"
                   ,element.szId
                   ,szSubordinateToId
                   ,element.szTitle);
           printf("TITLE: %s\n",element.szTitle); 
            //check to see if it is root node
            if (strcmp(szSubordinateToId,"ROOT")==0)
            {
                if (tree->pRoot == NULL)
                {
                    tree->pRoot = allocateNodeT(element);
                }
                else
                { 
                    tree->pRoot->pSibling = allocateNodeT(element);
                }
            }
            else
            {
                p = insertT(tree->pRoot,element,szSubordinateToId);
                
                //(error handling) if the parent node was not found
                if (p == NULL)
                    printf("Error, parent %s not found\n", szSubordinateToId);
            }
        }
        else if (strcmp(szToken,"VALUE")==0)
        {
            sscanf(pszInput, "%s %s %s %lf %[^\t\n]"
                   ,element.szId
                   ,szOptionId
                   ,&element.cCostInd
                   ,&element.dCost
                   ,element.szTitle);
            printf("VALUE TITLE: %s\n",element.szTitle);
            p = insertT(tree->pRoot,element,szOptionId);
            
            //(error handling)if the parent node was not found
            if (p == NULL)
                printf("DEFINE ERROR: parent %s not found\n", szSubordinateToId);
        }
    }
    else if (strcmp(szToken,"PRINT")==0)
    {
        pszInput = getToken(pszInput,szToken,MAX_TOKEN_SIZE);
        
        //if the command is to print all
        if (strcmp(szToken,"ALL")==0)
        {
            //Pretty print
            prettyPrintT(tree->pRoot,0);
        }
        //if the command is print one
        else
        {
            pszInput = getToken(pszInput,szToken,MAX_TOKEN_SIZE);
            printOne(tree, szToken);
        }
    }
    else if (strcmp(szToken,"QUOTE")==0)
    {
        //3 cases, BEGIN, OPTION,END
        pszInput = getToken(pszInput,szToken,MAX_TOKEN_SIZE);
        if (strcmp(szToken,"BEGIN")==0)
        {
            //idk
        }
        if (strcmp(szToken,"OPTION")==0)
        {
            sscanf(pszInput," %d %s %d"
                   ,&quoteSelection->quoteItemM[quoteSelection->iQuoteItemCnt].iLevel
                   ,quoteSelection->quoteItemM[quoteSelection->iQuoteItemCnt].szOptionId
                   ,&quoteSelection->quoteItemM[quoteSelection->iQuoteItemCnt].iSelection);
            quoteSelection->iQuoteItemCnt++;
        }
    }
    else if (strcmp(szToken,"DELETE")==0)
    {
        pszInput = getToken(pszInput,szToken,MAX_TOKEN_SIZE);
        p = findId(tree->pRoot,szToken);
        if (p == NULL)
            printf("DELETE ERROR: Id %s not found\n",szToken);
        else
        {
            deleteItem(tree,szToken);
        }
    }
    
}

/**************************************************************************
                                    QuoteEnd
 ***************************************************************************/

/******************************** quoteEND ********************************
 void quoteEND(Tree tree, char szId[])
 Purpose:
 
 Parameters:
 
 Returns:
 
 **************************************************************************
void quoteEND(Tree tree, char szId[])
{
    //Local Variable Declaration
    char szTitle[30] = tree->pRoot->element.szTitle;
    
    //Print Model
    while(szId == model)
    {
        if(tree->pRoot->element.szTitle == base) //Prints BASE Model
            printf("\t Base
                   \t\t\t\t
                   
                   else if(tree->pRoot->element.szTitle == lx) //Prints PLUS Model
                   printf("\t Plus\n");
                   else if(tree->pRoot->element.szTitle == oy) //Prints OH_YEAH! Model
                   printf("\t Oh_Yeah!\n");
                   else
                   printf("\nInvalid Model\n");
                   }
                   
                   //Print Engine
                   while(szId == engine)
                   {
                       if(tree->pRoot->element.szId == base) //Prints BASE Model
                           if
                               printf("\t\t\t Base\n");
                       
                           else
                               printf("\nInvalid Engine\n");
                       
                   }
                   
                   //Print Color
                   while(szId == color)
                   {
                       if(tree->pRoot->element.szId == base) //Prints BASE Model
                           if
                               printf("\t\t\t Base\n");
                       
                           else
                               printf("\nInvalid Engine\n");
                       
                   }
                   
                   //Print Audio
                   while(szId == audio)
                   {
                       if(tree->pRoot->element.szId == base) //Prints BASE Model
                           if
                               printf("\t\t\t Base\n");
                       
                           else
                               printf("\nInvalid Engine\n");
                       
                   }
                   
                   //Print Warranty
                   while(szId == warranty)
                   {
                       if(tree->pRoot->element.szId == base) //Prints BASE Model
                           if
                               printf("\t\t\t Base\n");
                       
                           else
                               printf("\nInvalid Engine\n");
                       
                   }
        }
*/
