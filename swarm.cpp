//UMBC - CSEE - CMSC 341 - Fall 2021 - Proj2
#include "swarm.h"
Swarm::Swarm(){
  m_root = nullptr;
}

Swarm::~Swarm(){
  clear();
}

void Swarm::insert(const Robot& robot){
  if (!findBot(robot.getID())){ //this could potentially slow down program might change later
    insert(robot, m_root);
  }else{
    cout << robot.getID() << " is already in the tree. We do not allow duplicates in the tree" << endl;
    }
}

void Swarm::insert(const Robot &robot, Robot*& aBot) {
  if (aBot == nullptr){
    Robot* newRobot = new Robot(robot.getID(), robot.getType(), robot.getState()); //makkes new node
    aBot = newRobot; //sets as new node  
    aBot->setHeight(0); //sets height to 0
  }else if(robot.getID() < aBot->getID()){ //goes to left
    insert(robot, aBot->m_left); 
  }else{ //goes to right
    insert(robot, aBot->m_right); 
  }
  updateHeight(aBot); //updates height 
  aBot = rebalance(aBot); //rebalances tree
}

void Swarm::clear(){
  clear(m_root); //calls helper func
}

void Swarm::clear(Robot*& aBot){ //helper function for clear
  if (aBot == nullptr) {  //base case
    return;
  }else{ //while tree is not empty
    clear(aBot->m_left);
    clear(aBot->m_right); //traverses tree
    delete aBot; //deletes node
    aBot = nullptr; //sets to nullptr
  }
}

void Swarm::remove(int id){ 
  m_root = remove(id, m_root); //calls helper func
  if(m_root){
    updateHeight(m_root); //updates height
    m_root = rebalance(m_root); //rebalances
  }
}

Robot* Swarm::remove(int id, Robot* aBot){
  if (aBot == nullptr) { 
    return nullptr;
  }else if(id < aBot->getID()){ //checks left
    aBot->m_left = remove(id, aBot->m_left); //recursive call that sets m_left
  }else if(id > aBot->getID()){ //checks if id is bigger and goes to right
    aBot->m_right = remove(id, aBot->m_right); //same on right side
  }else if((aBot->m_right) && (aBot->m_left)){ //if node has both children
    Robot* temp = findMin(aBot->m_right); //find the min value on right side and sets to temp
    aBot->setID(temp->getID()); //sets values
    aBot->setState(temp->getState()); //^
    aBot->setType(temp->getType());
    aBot->m_right = remove(aBot->getID(), aBot->m_right); //removes and sets m_right
  }else{ //if id is the root node id
    Robot* temp = aBot; //set temp node to root node
    if (aBot->m_left == nullptr) //traversal
      aBot = aBot->m_right;
    else
      aBot = aBot->m_left;
    delete temp; //deletes node
    temp = nullptr; 
  }
  return aBot;
}

void Swarm::updateHeight(Robot* aBot){
  aBot->setHeight(height(aBot)); //sets height 
}

int Swarm::height(Robot* aBot){ //helpert function to updateHeight
  if (aBot == nullptr){ 
    return -1;
  }else{
    //takes the height of biggest subtree and adds one to give height of current node
    return max(height(aBot->m_left), height(aBot->m_right)) + 1;
  }
}

int Swarm::checkImbalance(Robot* aBot){
  int leftHeight = -1; //keeps track of left
  int rightHeight = -1;  //keeps track of right
  if (aBot->getLeft()) 
    leftHeight = aBot->m_left->getHeight(); //updates left subtree height
  if (aBot->getRight()) 
    rightHeight = aBot->m_right->getHeight(); //updates right subtree height
  return (leftHeight - rightHeight); //returns balance factor
}

Robot* Swarm::rebalance(Robot* aBot){
  if ((checkImbalance(aBot) > 1) && (checkImbalance(aBot->m_left) >= 0)) { //left child and its left child have > height
    //Right rotation
    return rightRotation(aBot);
  } else if ((checkImbalance(aBot) < -1) && (checkImbalance(aBot->m_right) >= 0)) { //right node has > height but right nodes, left node has > height 
    //RightLeft rotation
    aBot->m_right = rightRotation(aBot->m_right);
    return leftRotation(aBot);
  } else if ((checkImbalance(aBot) < -1) && (checkImbalance(aBot->m_right) <= 0)) { //right child and its right child have > height
    //Left rotation
    return leftRotation(aBot);
  } else if ((checkImbalance(aBot) > 1) && (checkImbalance(aBot->m_left) <= 0)) { //left node has > height but left nodes, right node has > height
    //LeftRight rotation
    aBot->m_left = leftRotation(aBot->m_left);
    return rightRotation(aBot);
  } else { //if balanced
    return aBot; //simply return
  }   
}

Robot* Swarm::rightRotation(Robot *aBot) { //performs right rotation
  Robot* z = aBot;
  Robot* y = aBot->m_left;
  z->m_left = y->m_right;
  y->m_right = z; //reorganizes nodes
  updateHeight(z); //updates heights
  updateHeight(y);
  return y; //rerturns node after right rotation
}

Robot* Swarm::leftRotation(Robot *aBot) { //performs left rotation
  Robot* z = aBot;
  Robot* y = z->m_right;
  z->m_right = y->m_left;
  y->m_left = z;
  updateHeight(z); //updates height
  updateHeight(y); 
  return y; //returns node after left rotation
  
}

void Swarm::listRobots() const {
  inOrder(m_root); //calls helper function to print in order
}

bool Swarm::setState(int id, STATE state){ 
  if(findBot(id)){ //if bot exists
    Robot* theBot = getBot(id, m_root); //gets memory address of node with id 
    theBot->setState(state); //sets state
    return true; 
  }else{
    return false;
  }
}

Robot* Swarm::getBot(int id, Robot* aBot) {
  //returns pointer to node with specified id
  if (aBot->getID() == id) {
    return aBot;
  } else if (aBot->getID() > id) {
    getBot(id, aBot->getLeft());
  } else {
    getBot(id, aBot->getRight());
  }
}

Robot* Swarm::findMin(Robot* aBot){ //returns pointer to node with smallest value
  if ((aBot == nullptr) || (aBot->m_left == nullptr)){
    return aBot;
  }else{
    findMin(aBot->m_left); //keeps going left
  }
}

void Swarm::removeDead(){ 
  m_root = removeDead(m_root); //calls helper function
  if(m_root)
    updateHeight(m_root); //updates height after removal
}

Robot* Swarm::removeDead(Robot *aBot) { //helper function for removeDead 
  if (aBot) { 
    removeDead(aBot->m_left);
    removeDead(aBot->m_right); //traverses tree
    if (aBot->getState() == DEAD) { //if the robot is dead
      return remove(aBot->getID(), m_root); //remove it 
    }
  }
  return aBot; 
}

bool Swarm::findBot(int id) const { 
  return findBot(id, m_root);  //calls helper function 
}

bool Swarm::findBot(int id, Robot *aBot) const { //helper function for findBot
  if (aBot) { 
    if (aBot->getID() == id) {
      return true; //returns true if node has matching id
    } else if (aBot->getID() > id) {
      findBot(id, aBot->getLeft());
    } else {
      findBot(id, aBot->getRight());
    }
  }else{ //if id was not found
    return false; 
  }
}

void Swarm::dumpTree() const {
  dump(m_root); //calls helper function
}

void Swarm::dump(Robot* aBot) const{
  if (aBot != nullptr){
    cout << "(";
    dump(aBot->m_left);//first visit the left child
    cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
    dump(aBot->m_right);//third visit the right child
    cout << ")";
  }
}

void Swarm::inOrder(Robot* aBot) const{
  if (aBot){
    inOrder(aBot->m_left); //visits left child
    cout << aBot->getID() << ":" << aBot->getStateStr() <<  ":" << aBot->getTypeStr() << endl; //print values
    inOrder(aBot->m_right); //visit right child
  }
}
