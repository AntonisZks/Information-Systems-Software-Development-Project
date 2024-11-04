#include <iostream>
#include <set>
#include "../include/Evaluation/recall.h"
#include "../include/acutest.h"


void testRecallEvaluation_100_correct(){
    
  std::set<int> approximate = { 1, 2, 3, 4 };
  std::set<int> real = { 1, 2, 3, 4 };

  TEST_CHECK(calculateRecallEvaluation(approximate, real) > 0.50f);
  TEST_CHECK(calculateRecallEvaluation(approximate, real) > 0.95f);
  TEST_CHECK(calculateRecallEvaluation(approximate, real) == 1.0f);

}

void testRecallEvaluation_100_wrong(){
    
  std::set<int> approximate = { 1, 2, 3, 4 };
  std::set<int> real = { 5, 6, 7, 8 };

  TEST_CHECK(calculateRecallEvaluation(approximate, real) < 0.50f);
  TEST_CHECK(calculateRecallEvaluation(approximate, real) < 0.05f);
  TEST_CHECK(calculateRecallEvaluation(approximate, real) == 0.0f);

}

void testRecallEvaluation_50_correct(){
    
  std::set<int> approximate = { 1, 2, 3, 4 };
  std::set<int> real = { 1, 2, 7, 8 };

  TEST_CHECK(calculateRecallEvaluation(approximate, real) > 0.4f);
  TEST_CHECK(calculateRecallEvaluation(approximate, real) < 0.6f);
  TEST_CHECK(calculateRecallEvaluation(approximate, real) == 0.5f);

}

TEST_LIST = {
    {"Test Recall Evaluation 100% corrent", testRecallEvaluation_100_correct},
    {"Test Recall Evaluation 100% wrong", testRecallEvaluation_100_wrong},
    {"Test Recall Evaluation 50% correct", testRecallEvaluation_50_correct},
    {nullptr, nullptr} // Termination
};
