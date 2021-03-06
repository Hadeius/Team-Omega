#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "cs2123p5.h"

/************************** traverseQuote *******************************
 NodeT *traverseQuote(NodeT *p)
 Purpose:
        Traverses the tree in the quote looking for the level 2 option node
 Parameters:
        I NodeT *p                      starting node
·
 Returns:
        p           found
        temp        found and recursing
        NULL        not found
 **************************************************************************/

NodeT *traverseQuote(NodeT *p)
{
    NodeT *temp = NULL;

    //if the quote is null
    if (p==NULL)
        return NULL;
    
    //If the option node is found
    if (p->element.cNodeType!='O')
    {
        //traverse siblings if sibling is not null
        if (p->pSibling != NULL)
            temp = traverseQuote(p->pSibling);
        //if node is found
        if (temp !=NULL)
            return temp;
        //if node is not found through siblings, traverse the children
        if (p->pChild != NULL)
            temp = traverseQuote(p->pChild);
        return temp;
    }
    //it is found
    else if (p->element.cNodeType=='O')
        return p;
    return NULL;

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
    int bCheck;
    QuoteSelection partial = newQuoteSelection();
    QuoteResult result;
    NodeT *pRoot;
    NodeT *pFind;
    int iChildCnt = 0;
    int iChildCnt2 = 0;
    int iCount = 0;
    int i = 0;
    double dTotal = 0;

    printf("\n");
    while (i < quoteSelection->iQuoteItemCnt)
    {
        int q = 1;
        
        //level 0
        if (quoteSelection->quoteItemM[i].iLevel == 0)
        {
            //check to see if partial
            if (iChildCnt != 0) 
            {
                int h;
                
                //to find which node is missing
                h = iChildCnt2 - iChildCnt;
                
                //printf("\tPartial Total is \t\t\t\t$%.2lf\n",result.dTotalCost);
                //printf("PARTIAL QUOTE: Missing %s\n", partial->quoteItemM[h].szOptionId);
                
                //return code for partial
                result.returnCode = 1;
                result.error = partial->quoteItemM[h];
                return result;
            }

            //check for a duplicate entry
            int u=0;
            while (u < i)
            {

                //if current quote is found to match any previous quotes
                if (strcmp(quoteSelection->quoteItemM[i].szOptionId,quoteSelection->quoteItemM[u].szOptionId)==0)
                {
                    //too many arguments error
                    result.returnCode = 2;
                    result.error = quoteSelection->quoteItemM[i];
                    return result;
                }
                u++;
            }
            //find the root node
            pRoot = findId(tree->pRoot,quoteSelection->quoteItemM[i].szOptionId);
            
            //if root is not found
            if (pRoot == NULL)
            {
                result.returnCode = 2;
                result.error = quoteSelection->quoteItemM[i];
                return result;
            }

            //print the title
            printf("  %s\n",pRoot->element.szTitle);
            pRoot = pRoot->pChild;
            
            //find node for the selection
            while (q < quoteSelection->quoteItemM[i].iSelection)
            {
                pRoot = pRoot->pSibling;
                q++;
            }
            
            //record values to check for a partial quote
            if (pRoot->pChild != NULL)
            {
                bCheck = TRUE;
                NodeT *pTemp = pRoot->pChild;
                NodeT *pCheck = NULL;
                while (pTemp !=NULL)
                {
                    strcpy(partial->quoteItemM[iChildCnt].szOptionId, pTemp->element.szId);
                    
                    //check for level 2 nodes
                    pCheck = traverseQuote(pTemp->pChild);
                    if (pCheck != NULL)
                    {
                        iChildCnt++;
                        iChildCnt2++;
                        partial->quoteItemM[iChildCnt].iLevel = 2;
                        pTemp = pTemp->pSibling;

                        strcpy(partial->quoteItemM[iChildCnt].szOptionId, pCheck->element.szId);
                        iChildCnt++;
                        iChildCnt2++;
                    }
                    //not a level 2 node
                    else
                    {
                        partial->quoteItemM[iChildCnt].iLevel = 1;
                        pTemp = pTemp->pSibling;
                        iChildCnt++;
                        iChildCnt2++;
                    }
                }

            }
            else
                bCheck = FALSE;
            
            //print
            prettyPrintPartial(pRoot,quoteSelection,0);
            result.dTotalCost += pRoot->element.dCost;

        }
        //level 1 nodes
        else if (quoteSelection->quoteItemM[i].iLevel == 1)
        {
            //check for duplicates
            int u=0;
            while (u < i)
            {
                if (strcmp(quoteSelection->quoteItemM[i].szOptionId,quoteSelection->quoteItemM[u].szOptionId)==0)
                {
                    //too many arguments error
                    result.returnCode = 2;
                    result.dTotalCost = dTotal;
                    result.error = quoteSelection->quoteItemM[i];
                    return result;
                }
                u++;
            }

            pFind = findId(pRoot->pChild,quoteSelection->quoteItemM[i].szOptionId);
            //check to see if the option was not found
            if (pFind == NULL)
            {
                result.returnCode = 2;
                result.error = quoteSelection->quoteItemM[i];
                result.dTotalCost = dTotal;
                return result;
            }
            //reduce the child count if the node is in correct order. (partial checking)
            if (bCheck && strcmp(partial->quoteItemM[iCount].szOptionId,pFind->element.szId)==0)
            {
                iChildCnt--;
            }
            //if its not accounted for, then its a partial quote.
            else if (bCheck && strcmp(partial->quoteItemM[iCount].szOptionId,pFind->element.szId)!=0)
            {
                result.returnCode = 1;
                result.error = partial->quoteItemM[iCount];
                return result;

            }
            printf("           %s\n",pFind->element.szTitle);
            pFind = pFind->pChild;

            //check to make sure selection value is not greater than # of nodes
            while (q < quoteSelection->quoteItemM[i].iSelection)
            {
                if (pFind->pSibling == NULL)
                {
                    result.error = quoteSelection->quoteItemM[i];
                    result.dTotalCost = dTotal;
                    result.returnCode = 3;
                    return result;
                }
                pFind = pFind->pSibling;
                q++;
            }
            prettyPrintPartial(pFind,quoteSelection,0);
            result.dTotalCost += pFind->element.dCost;
            iCount++;
        }
        else if (quoteSelection->quoteItemM[i].iLevel == 2)
        {
            //check to see if duplicate
            int u=0;
            while (u < i)
            {
                if (strcmp(quoteSelection->quoteItemM[i].szOptionId,quoteSelection->quoteItemM[u].szOptionId)==0)
                {
                    //too many arguments error
                    result.returnCode = 2;
                    result.dTotalCost = dTotal;
                    result.error = quoteSelection->quoteItemM[i];
                    return result;
                }
                u++;
            }
    
            //check to see if the option value was not found
            pFind = findId(tree->pRoot,quoteSelection->quoteItemM[i].szOptionId);
            if (pFind == NULL)
            {
                printf("Bad option value\n");
                result.returnCode = 2;
                result.error = quoteSelection->quoteItemM[iCount];
                result.dTotalCost = dTotal;
                return result;
            }
            //reduce childcnt if the node is accounted for
            if (bCheck && strcmp(partial->quoteItemM[iCount].szOptionId,pFind->element.szId)==0)
            {
                iChildCnt--;
            }
            //if node is not accounted for, then it is a partial quote
            else if (bCheck && strcmp(partial->quoteItemM[iCount].szOptionId,pFind->element.szId)!=0)
            {
                result.returnCode = 1;
                result.error = partial->quoteItemM[iCount];
                return result;

            }
            printf("           %s\n",pFind->element.szTitle);
            pFind = pFind->pChild;

            //check to see if selection value is too high 
            while (q < quoteSelection->quoteItemM[i].iSelection)
            {
                if (pFind->pSibling == NULL)
                {
                    result.error = quoteSelection->quoteItemM[i];
                    result.returnCode = 3;
                    return result;
                }
                pFind = pFind->pSibling;
                q++;
            }
            prettyPrintPartial(pFind,quoteSelection,0);
            result.dTotalCost += pFind->element.dCost;
            iCount++;

        }

        //increment count
        i++;
    }
    //print total
    free(partial);
    result.returnCode = 0;
    return result;
}

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
    
    //if tree passed in is NULL
    if (p == NULL)
        return NULL;

    //pkid has been found
    if (strcmp(p->element.szId,pkid->element.szId)==0)
        return pParent;

    //traverse sibling nodes
    if (p->pSibling !=NULL)
        pFound = findParent(pParent,p->pSibling,pkid);
    
    //pkid has been found so dont traverse through children
    if (pFound != NULL)
        return pFound;
    
    //traverse child nodes
    if (p->pChild != NULL)
        pFound = findParent(p,p->pChild,pkid);

    //if found in child nodes, returns the found node, else it returns NULL
    return pFound;
}

/********************************* processCommand *******************************
 void processCommand(Tree tree, QuoteSelection quoteSelection, char *pszInput)
 
 Purpose: to read input file and
 
 Parameters:
 
 Returns:
 
 ********************************************************************************/
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
            element.cNodeType = 'O';
            sscanf(pszInput, "%s %s %[^\t\n]"
                   ,element.szId
                   ,szSubordinateToId
                   ,element.szTitle);
 
            //check to see if it is root node
            if (strcmp(szSubordinateToId,"ROOT")==0)
            {

                if (tree->pRoot == NULL)
                    tree->pRoot = allocateNodeT(element);
                else
                    tree->pRoot->pSibling = allocateNodeT(element);
            }
            else
            {
                insertPriceMenu(tree,element,szSubordinateToId);
            }
        }
        else if (strcmp(szToken,"VALUE")==0)
        {
            element.cNodeType = 'V';
            sscanf(pszInput, "%s %s %s %lf %[^\t\n]"
                   ,element.szId
                   ,szOptionId
                   ,&element.cCostInd
                   ,&element.dCost
                   ,element.szTitle);
            if (strcmp(szOptionId,"ROOT")==0)
                printf("DEFINE ERROR: ROOT is not option\n");
            else
                insertPriceMenu(tree,element,szOptionId);
        }
    }
    else if (strcmp(szToken,"PRINT")==0)
    {
        pszInput = getToken(pszInput,szToken,MAX_TOKEN_SIZE);
        
        //if the command is to print all
        if (strcmp(szToken,"ALL")==0)
        {
            //Pretty print
            printPriceMenu(tree);
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
            quoteSelection->iQuoteItemCnt = 0;
        }
        if (strcmp(szToken,"OPTION")==0)
        {
            sscanf(pszInput," %d %s %d"
                   ,&quoteSelection->quoteItemM[quoteSelection->iQuoteItemCnt].iLevel
                   ,quoteSelection->quoteItemM[quoteSelection->iQuoteItemCnt].szOptionId
                   ,&quoteSelection->quoteItemM[quoteSelection->iQuoteItemCnt].iSelection);
            quoteSelection->iQuoteItemCnt++;
        }
        if (strcmp(szToken,"END")==0)
        {
            QuoteResult result;
            result = determineQuote(tree,quoteSelection);
            switch (result.returnCode)
            {
                case 0:
                    printf("\t\tTotal Cost \t\t\t\t$%.2lf\n",result.dTotalCost);
                    break;
                case 1:
                    printf("\tPartial Total is \t\t\t\t$%.2lf\n",result.dTotalCost);
                    printf("PARTIAL QUOTE: missing %s", result.error.szOptionId);
                    break;
                case 2:
                    printf("Bad option value: %s\n"
                            ,result.error.szOptionId);
                    break;
                case 3:
                    printf("Bad Selection value: %s %d\n"
                            ,result.error.szOptionId
                            ,result.error.iSelection);
                    break;
            }
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

void insertPriceMenu(Tree tree, Element element, char szParentId[])
{
    NodeT *p;
    p = findId(tree->pRoot, element.szId);
    //the Id already exists
    if (p != NULL)
    {
        printf("DEFINE ERROR: %s already exists\n", element.szId);
        return;
    }
    p = findId(tree->pRoot, szParentId);
    //parent was not found
    if (p == NULL)
    {
        printf("DEFINE ERROR: parent %s not found\n", szParentId);
        return;
    }

    //value into a value
    if (element.cNodeType == 'V' && p->element.cNodeType == 'V' )
    {
        printf("DEFINE ERROR: Inserting value node into a value node\n");
        return;
    }
    //option into an option
    if (element.cNodeType == 'O' && p->element.cNodeType == 'O' )
    {
        printf("DEFINE ERROR: Inserting option node into a option node\n");
        return;
    }
    insertT(tree->pRoot,element,szParentId);
}
