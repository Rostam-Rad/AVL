#include "swarm.h"


class Tester{
public:
  bool testInsert();
  bool testBalance();
  bool testAllProp();
  bool testRemove();
  bool testRemoveDead();
  bool testDuplicate();
  void testInsertSpeed(int numNodes);
  void testRemoveSpeed(int numNodes);
};

int main(){
  Tester test;
    if (test.testInsert())
      cout << "*ALL TESTS PASSED*" << endl;
    else
      cout << "TEST FAILED" << endl;
    if (test.testBalance())
      cout << "*ALL TESTS PASSED*" << endl;
    else
      cout << "TEST FAILED" << endl;
    if (test.testAllProp())
      cout << "*ALL TESTS PASSED*" << endl;
    else
      cout << "TEST FAILED" << endl;
    if (test.testRemove())
      cout << "*ALL TESTS PASSED*" << endl;
    else
      cout << "TEST FAILED" << endl;
    if (test.testRemoveDead()) {
      cout << "*ALL TESTS PASSED*" << endl;
    }else
      cout << "TEST FAILED" << endl;
    if (test.testDuplicate()) {
      cout << "DUPLICATE TEST PASSED" << endl;
    }else
      cout << "TEST FAILED" << endl;
    cout << "Testing Insertion Performance: " << endl;
    test.testInsertSpeed(1000);
    test.testInsertSpeed(2000);
    test.testInsertSpeed(4000);
    test.testInsertSpeed(10000);
    cout << "Testing Removal Performance: " << endl;
    test.testRemoveSpeed(1000);
    test.testRemoveSpeed(2000);
    test.testRemoveSpeed(4000);
    test.testRemoveSpeed(10000);
    
    return 0;
};
bool Tester::testInsert() {
  cout << "TESTING INSERT FUNCTION: " ;
  Swarm s;
  int numNodes = 100; //number of nodes you want to add
  for (int i = 0; i < numNodes; i++){  //adds 20 nodes
    Robot temp(i);
    s.insert(temp);
  }
  for (int i = 0; i < numNodes; i++){ //checks to if all 20 nodes exits
    if (!s.findBot(i)){
      return false;
    }
  }
  return true;
}

bool Tester::testBalance() {
  cout << "TESTING REBALANCE FUNCTION: ";
  Swarm s;
  int tru = 0;
  int numNodes = 20; //number of nodes you want to add
  for (int i = 0; i < numNodes; i++){  //adds 20 nodes
    Robot temp(i);
    s.insert(temp);
  }
  if (s.getBot(7, s.m_root)->getHeight() == 4){
    if (s.getBot(15, s.m_root)->getHeight() == 3){
      tru += 1;
    }
  }
  Swarm s1;
  numNodes += 30; //number of nodes you want to add
  for (int i = 0; i < numNodes; i++){  //adds 20 nodes
    Robot temp(i);
    s1.insert(temp);
  }
  if (s1.getBot(31, s1.m_root)->getHeight() == 5){
    if (s1.getBot(15, s1.m_root)->getHeight() == 4){
            if (s1.getBot(5, s1.m_root)->getHeight() == 1) {
	      tru += 1;
            }
    }
  }
  if (tru == 2){
    return true;
  }
  return false;
}

bool Tester::testAllProp() {
  cout << "Testing Whether The BST Property Is Preserved After All Insertions: ";
    Swarm s;
    int numNodes = 20;
    for (int i = 0; i < numNodes; i++) {  //adds 20 nodes
      Robot temp(i);
      s.insert(temp);
    }
    s.remove(15);
    s.remove(16);
    s.remove(17);
    s.remove(18);
    s.remove(19);
    for (int i = 0; i < 15; i++) {
      if (!s.findBot(i)) {
	return false;
      }
    }
    s.remove(0);
    s.remove(1);
    s.remove(2);
    s.remove(3);
    s.remove(4);
    for (int i = 5; i < 15; i++) {
      if (!s.findBot(i)) {
	return false;
      }
    }
    s.remove(5);
    s.remove(6);
    s.remove(7);
    s.remove(8);
    s.remove(9);
    s.remove(0);
    s.remove(11);
    s.remove(12);
    s.remove(13);
    s.remove(14);
    
    s.insert(11);
    s.insert(12);
    for (int i = 11; i < 12; i++) {
      if (!s.findBot(i)) {
	return false;
      }
    }
    return true;
}

bool Tester::testRemove() {
  cout << "Testing Remove Function: ";
  Swarm s;
  int tru = 0;
  int numNodes = 20;
  for (int i = 0; i < numNodes; i++) {  //adds 20 nodes
    Robot temp(i);
    s.insert(temp);
  }
    s.remove(8);
    s.remove(16);
    s.remove(10);
    if (!s.findBot(8) && !s.findBot(16) && !s.findBot(10))
        tru += 1;
    //testing if tree is still balanced after remove
    s.insert(20);
    if (s.getBot(7, s.m_root)->getHeight() == 4) {
      tru += 1;
    }
    if (tru == 2) {
        return true;
    }
    return false;
}

bool Tester::testRemoveDead() {
  cout << "Testing Remove Dead Function: ";
  Swarm s;
  int numNodes = 50;
  for (int i = 0; i < numNodes; i++) {  //adds 20 nodes
    Robot temp(i);
    s.insert(temp);
    }
  s.removeDead();
  for (int i = 0; i < numNodes; i++){
    if (!s.findBot(i)){
      return false;
    }else{
      s.getBot(i,s.m_root)->setState(DEAD);
    }
  }
  s.removeDead();
    for (int i = 0; i < numNodes; i++) {
      if (s.findBot(i)) {
	return false;
      }
    }
    return true;
}

bool Tester::testDuplicate() {
  cout << "Testing Duplicates: " << endl;
  Swarm s;
  int numNodes = 10; //number of nodes you want to add
  for (int i = 0; i < numNodes; i++) {  //adds 20 nodes
    Robot temp(i);
    s.insert(temp);
  }
  s.insert(3);
  s.insert(4);
  s.insert(5);
  return true; 
}

void Tester::testInsertSpeed(int numNodes){
  clock_t start, stop;//stores the clock ticks while running the program
  double T = 0.0;//to store running times
  Swarm s;
  start = clock();
  for (int i = 0; i < numNodes; i++) {
    Robot temp(i);
    s.insert(temp);
  }
  stop = clock();
  T = stop - start;
  cout << "Inserting " << numNodes << " items took " << T << " clock ticks (" << T / CLOCKS_PER_SEC << " seconds)!" << endl;
  
}

void Tester::testRemoveSpeed(int numNodes){
  Swarm s;
  for (int i = 0; i < numNodes; i++) {  //adds 20 nodes
    Robot temp(i);
    s.insert(temp);
  }
  clock_t start, stop;//stores the clock ticks while running the program
  double T = 0.0;//to store running times
  start = clock();
  for (int i = 0; i < numNodes; i++) {
    s.remove(i);
  }
  stop = clock();
  T = stop - start;
  cout << "Removing " << numNodes << " items took " << T << " clock ticks (" << T / CLOCKS_PER_SEC << " seconds)!" << endl;
}


