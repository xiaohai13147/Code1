// methods
static Trnode* MakeNode(const Item* pi)
{
	Trnode* pt = (Trnode*)malloc(sizeof(Trnode));
	if(pt != NULL)
	{
		pt->item = *pi;
		pt->left = NULL;
		pt->right = NULL;
	}
	return pt;
}
static void AddNode(Trnode* new_node, Trnode* root)
{
	if(ToLeft(&new_node->item, &root->item))
	{
		if(root->left == NULL)
			root->left = new_node;
		else
			AddNode(new_node, root->left);
	}
		
	else if(ToRight(&new_node->item, &root->item))
	{
		if(root->right == NULL)
			root->right = new_node;
		else
			AddNode(new_node, root->right);
	}
	else
	{
		fprintf(stderr, "location error in AddNode()\n");
		exit(1);
	}
}
static bool ToLeft(const Item* i1, const Item* i2)
{
	int comp1;
	if((comp1 = strcmp(i1->petname, i2->petname)) < 0)
		return true;
	else if(comp1 == 0 && strcmp(i1->petkind, i2->petkind) < 0)
		return true;
	else 
		return false;
}
static bool ToRight(const Item* i1, const Item* i2)
{
	int comp1;
	if((comp1 = strcmp(i1->petname, i2->petname)) > 0)
		return true;
	else if(comp1 == 0 && strcmp(i1->petkind, i2->petkind) > 0)
		return true;
	else 
		return false;
}
typedef struct pair
{
	Trnode* parent;
	Trnode* child;
}Pair;
static Pair SeekItem(const Item* pi, const Tree* ptree)
{
	Pair look;
	look.parent = NULL;
	look.child = ptree->root;
	if(look.child == NULL)
		return look;
	while(look.child != NULL)
	{
		if(ToLeft(pi, &(look.child->item)))
		{
			look.parent = look.child;
			look.child = look.child->left;
		}
		else if(ToRight(pi, &(look.child->item)))
		{
			look.parent = look.child;
			look.child = look.child->right;
		}
		else
			break;
	}
	return look;
}
void InitializeTree(Tree* ptree)
{
	ptree->root = NULL;
}
bool TreeIsEmpty(const Tree* ptree)
{
	return ptree->size == 0;
}
bool TreeIsFull(const Tree* ptree)
{
	return ptree->size == MAXTEMS;
}
int TreeItemCount(const Tree* ptree)
{
	return ptree->size;
}
bool AddItem(const Item* pi, Tree* ptree)
{
	if(TreeIsFull(ptree))
		return false;
	if(SeekItem(pi, tree).child != NULL)
		return false;
	Trnode* current = MakeNode(pi);
	if(current == NULL)
		return false;
	ptree->size++;
	if(ptree->root == NULL)
		ptree->root = current;
	else
		AddNode(current, ptree->root);
	return true;
}
bool Intree(const Item* pi, const Tree* ptree)
{
	return (SeekItem(pi, ptree).child == NULL) ? false : true;
}
static void DeleteNode(Trnode** ptr) // ptr = parent.left(right) - 目标节点
{
    Trnode* temp;
    if((*ptr)->left == NULL)         // 没有左子节点的节点
    {
        temp = *ptr;
        *ptr = (*ptr)->right;
        free(temp);
    }
    else if((*ptr)->right == NULL)  // 没有右子节点的节点
    {
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
    else                            // 有两个子节点的节点
    {
        // 连接两个子树
        for(temp = (*ptr)->left; temp->right != NULL; temp = temp->right)
            continue;
        temp->right = (*ptr)->right;
        // 连接父节点和新的子树
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
}
bool DeleteItem(const Item* pi, Tree* ptree)
{
    Pair look;
    look = SeekItem(pi, ptree);
    if(look.child == NULL)
        return false;
    if(look.parent == NULL)
        DeleteNode(&ptree->root);
    else if(look.parent->left == look.child)
        DeleteNode(&look.parent->left);
    else
        DeleteNode(&look.parent->right);
    ptree->size--;
    return true;
}
void Traverse(const Tree* ptree, void(*pfun)(Item item))
{
	if(ptree != NULL)
		InOrder(ptree->root, pfun);
}
static void InOrder(const Trnode* root, void(*pfun)(Item item))
{
	if(root != NULL)
	{
		InOrder(root->left, pfun);
		(*pfun)(root->item);
		InOrder(root->right, pfun);
	}
}
void DeleteAll(Tree* ptree)
{
	if(ptree != NULL)
		DeleteAllNodes(ptree->root);
	ptree->root = NULL;
	ptree->size = 0;
}
static void DeleteAllNodes(Trnode* root)
{
	Trnode* pright;
	if(root != NULL)
	{
		pright = root->right;
		DeleteAllNodes(root->left);
		free(root);
		DeleteAllNodes(pright);
	}
}