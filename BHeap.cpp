#include <iostream>

using namespace std;

template<typename keytype>
class BHeap{
private:
    struct Node{
        keytype key;
        int rank = 0;
        //Node* children[10] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        Node* leftChild = nullptr;
        Node* next = nullptr;
        Node* prev = nullptr;
        int marker = 0;
    };

   void consolidate(){
        Node* rankArray[30] = {nullptr};
        Node* curr = min;

        for(int a = 0; a < rootListSize; a++){
            int rankVal = curr->rank;
            Node* nextNode = curr->next;

            for(int b = 0; b < 30; b++){
                if(rankArray[b] == nullptr && curr->rank == b){
                    rankArray[b] = curr;
                    break;
                }
                else if(rankArray[b] != nullptr && curr->rank == b){
                    Node* arrayNode = rankArray[b];
                    if(curr->key < arrayNode->key){
                        if(curr->leftChild == nullptr){
                            curr->leftChild = arrayNode;
                            curr->leftChild->next = nullptr;
                            curr->leftChild->prev = nullptr;
                            curr->rank++;
                            rankVal++;
                            rankArray[rankVal-1] = nullptr;
                        }
                        else if(curr->leftChild != nullptr && curr->leftChild->next == nullptr){
                            curr->leftChild->next = arrayNode;
                            curr->leftChild->prev = arrayNode;
                            arrayNode->next = curr->leftChild;
                            arrayNode->prev = curr->leftChild;
                            curr->rank++;
                            rankVal++;
                            rankArray[rankVal-1] = nullptr;
                        }
                        else{
                            curr->leftChild->prev->next = arrayNode;
                            arrayNode->prev = curr->leftChild->prev;
                            curr->leftChild->prev = arrayNode;
                            arrayNode->next = curr->leftChild;
                            curr->rank++;
                            rankVal++;
                            rankArray[rankVal-1] = nullptr;
                        }
                    }
                    else{
                        if(arrayNode->leftChild == nullptr){
                            arrayNode->leftChild = curr;
                            curr = arrayNode;
                            curr->leftChild->next = nullptr;
                            curr->leftChild->prev = nullptr;
                            curr->rank++;
                            rankVal++;
                            rankArray[rankVal-1] = nullptr;
                        }
                        else if(arrayNode->leftChild != nullptr && arrayNode->leftChild->next == nullptr){
                            arrayNode->leftChild->next = curr;
                            arrayNode->leftChild->prev = curr;
                            curr->next = arrayNode->leftChild;
                            curr->prev = arrayNode->leftChild;
                            curr = arrayNode;
                            curr->rank++;
                            rankVal++;
                            rankArray[rankVal-1] = nullptr;
                        }
                        else {
                            arrayNode->leftChild->prev->next = curr;
                            curr->prev = arrayNode->leftChild->prev;
                            arrayNode->leftChild->prev = curr;
                            curr->next = arrayNode->leftChild;
                            curr = arrayNode;
                            curr->rank++;
                            rankVal++;
                            rankArray[rankVal-1] = nullptr;
                        }
                    }
                }
            }
            curr = nextNode;
        }

        //insert nodes back into list
        min = nullptr;
        int oldSize = rootListSize;
        rootListSize = 0;
        for(int a = 0; a < 30; a++){
            if(rootListSize == oldSize) break;
            if(rankArray[a] == nullptr){
                continue;
            }
            if(min == nullptr && rankArray[a] != nullptr){
                min = rankArray[a];
                min->next = nullptr;
                min->prev = nullptr;
                rootListSize++;
            }
            else if(min->next == nullptr && rankArray[a] != nullptr){
                min->next = rankArray[a];
                min->prev = rankArray[a];
                rankArray[a]->prev = min;
                rankArray[a]->next = min;
                if(rankArray[a]->key < min->key){
                    min = rankArray[a];
                }
                rootListSize++;
            }
            else if(rankArray[a] != nullptr){
                min->prev->next = rankArray[a];
                rankArray[a]->prev = min->prev;
                min->prev = rankArray[a];
                rankArray[a]->next = min;
                if(rankArray[a]->key < min->key){
                    min = rankArray[a];
                }
                rootListSize++;
            }
        }

    }
   
    Node* min = nullptr;
    int rootListSize;
    //int printCalls;

public:

    BHeap(){
        min = nullptr;
        rootListSize = 0;
    }
    
    BHeap(keytype k[], int s){
        min = nullptr;
        rootListSize = 0;
        for(int i = 0; i < s; i++){
            insert(k[i]);
        }  
        consolidate();
    }
    
    keytype peekKey(){
        return min->key;
    }
    
    keytype extractMin(){
        if(rootListSize == 1 && min->leftChild == nullptr){
            keytype minKey = min->key;
            min = nullptr;
            rootListSize = 0;
            return minKey;
        }
        rootListSize--;
        keytype minKey = min->key;
        Node* curr = min->prev;
        //move children to root list and update next and prev pointers
        if(min->leftChild != nullptr){
            if(min->next == nullptr){
                rootListSize = min->rank;
                min = min->leftChild;
            }
            else if(min->leftChild->next == nullptr){
                min->leftChild->prev = min->prev;
                min->leftChild->next = min->next;
                min->prev->next = min->leftChild;
                min->next->prev = min->leftChild;
                rootListSize++;
                min = min->leftChild;
            }
            else{
                min->prev->next = min->leftChild;
                min->next->prev = min->leftChild->prev;
                min->leftChild->prev->next = min->next;
                rootListSize += min->rank;
                min = min->leftChild; 
            }
        }
        else{
            min->prev->next = min->next;
            min->next->prev = min->prev;
            min = min->next;
        }

        consolidate();
        return minKey;
    }
    
    void insert(keytype k){
        rootListSize++;
        //if tree is empty, create first node
        if(min == nullptr){
            Node* node = new Node();
            node->key = k;
            node->next = nullptr;
            node->prev = nullptr;
            //update childrne pointers
            min = node;
            //cout << "created first node";
            return;
        }
        else if(min->next == nullptr){ //check to see if min is only node in list
            Node* node = new Node();
            min->prev = node;
            min->next = node;
            node->prev = min;
            node->next = min;
            node->key = k;
            if(k < min->key){
                min = node;
            }
            return;
        }
        //else update all pointers and insert new node
        Node* node = new Node();
        min->prev->next = node;
        node->prev = min->prev;
        min->prev = node;
        node->next = min;
        node->key = k;
        if(k < min->key){
            min = node;
        }
    }

    void merge(BHeap<keytype> &H2){
        //make min pointer in H2 null to empty list
        H2.min->prev->next = min;
        min->prev->next = H2.min;
        H2.min->prev = min->prev;
        min->prev = H2.min;
        if(H2.min->key < min->key){
            min = H2.min;
        }
        rootListSize += H2.rootListSize;
        H2.min = nullptr;
        H2.rootListSize = 0;
    }
   
    void printKeyHelper(Node *node) {
        if (node) {
            Node *firstNode = node->leftChild;
            Node *child = node->leftChild;
            cout << node->key << " ";
            if (child) {
                do {
                    printKeyHelper(child);
                    if(child->next != nullptr) child = child->next; // Move to the next child
                } while (child != firstNode && child);
            }
        }
    }

    void printKey() {
        if(min == nullptr) return;
        Node *curr = min;
        do {
            cout << "B" << curr->rank << ": " <<endl;
            printKeyHelper(curr);
            cout << endl;
            if(curr->next != nullptr) curr = curr->next;
        } while (curr != min);
    }

    // Destructor
    ~BHeap(){
        // min->next = nullptr;
        // min->prev = nullptr;
        // min->leftChild = nullptr;
        min = nullptr;
        //cout << "destructor called " << endl;
    }

    // Copy Constructor
    BHeap(const BHeap &old) {
        min = old.min;
        rootListSize = old.rootListSize;
        //cout << " copy constructor called " << endl;
    }
    
    // Copy Assignment Operator
    BHeap& operator=(const BHeap& rhs) {
        if(this != &rhs){
            //deleteHeap();
            rootListSize = rhs.rootListSize;
            min = rhs.min;
        }
        //cout << " copy assighment operator called " << endl;

        return *this;
    }

};
