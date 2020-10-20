# HW3: Testy cache
Arthur Lawson, Ian Jackson

## Goal:  
Practice unit testing skills by beefing up our testing script, then running it on our cache and others'.

**Project Pairs and Folder Names:**
Arthur & Ian - "hw2"
Danny & Hannah - "Systems_HW2-master"
Aryeh & Connor - "HW2AryehStahlAndConnorDeiparine-master"
Max & Kai - "hash-it-out-master"

## Part 1: Testing Our Code

We added the following tests to our test script, with the following results when run against our Cache implementation.
```
+-------------------------------+-----------------------------------------+--------+
| Test Name                     | Description                             | Status |
+-------------------------------+-----------------------------------------+--------+
| test_set                      | tests functionality of set function     | PASS   |
+-------------------------------+-----------------------------------------+--------+
| test_get                      | tests functionality of get function     | PASS   |
+-------------------------------+-----------------------------------------+--------+
| test_reset                    | tests functionality of reset function   | PASS   |
+-------------------------------+-----------------------------------------+--------+
| test_no_key                   | tests get function with empty cache     | PASS   |
+-------------------------------+-----------------------------------------+--------+
| test_changed_key              | tests that key value gets changed when  | PASS   |
|                               | key is overriden                        |        |
+-------------------------------+-----------------------------------------+--------+
| test_changed_key_size         | tests that get function return size is  | PASS   |
|                               | correct                                 |        |
+-------------------------------+-----------------------------------------+--------+
| test_evictor                  | tests basic functionality of evictor    | PASS   |
+-------------------------------+-----------------------------------------+--------+
| test_space_used               | tests functionality of space used       | PASS   |
+-------------------------------+-----------------------------------------+--------+
| test_word_exceed              | tests inserting value that is larger    | FAIL   |
|                               | than cache itself                       |        |
+-------------------------------+-----------------------------------------+--------+
| test_evictor_twice            | tests that evictor works with multiple  | PASS   |
|                               | evictions                               |        |
+-------------------------------+-----------------------------------------+--------+
| test_evictor_empty            | make sure that evictor doesn't          | PASS   |
|                               | cause bugs after cache is reset         |        |
+-------------------------------+-----------------------------------------+--------+
| test_evictor_with_del         | make sure that evictor works            | PASS   |
|                               | properly after key is deleted           |        |
+-------------------------------+-----------------------------------------+--------+
| test_evictor_with_repeat_keys | tests evictor to make sure it           | PASS   |
|                               | works well with repeated values         |        |
|                               | that eventually exceed cache size       |        |
+-------------------------------+-----------------------------------------+--------+
```
Our test_word_exceeded function failed due to the fact that one of our conditional statements that checked for a size-too-large condition both caused an eviction, but then continued to add the item to the Cache's storage anyway. This issue was fixed.

(Our test code has comments for whether each test is evictor or cache specific.)

## Part 2: Testing Others' Code


#### Kai/Max (hash-it-out):
```
+-------------------------------+--------+
| Test Name                     | Status |
+-------------------------------+--------+
| test_set                      | PASS   |
+-------------------------------+--------+
| test_get                      | PASS   |
+-------------------------------+--------+
| test_reset                    | FAIL   |
+-------------------------------+--------+
| test_no_key                   | FAIL   |
+-------------------------------+--------+
| test_changed_key              | FAIL   |
+-------------------------------+--------+
| test_changed_key_size         | FAIL   |
+-------------------------------+--------+
| test_evictor                  | FAIL   |
+-------------------------------+--------+
| test_space_used               | PASS   |
+-------------------------------+--------+
| test_word_exceed              | FAIL   |
+-------------------------------+--------+
| test_evictor_twice            | FAIL   |
+-------------------------------+--------+
| test_evictor_empty            | FAIL   |
+-------------------------------+--------+
| test_evictor_with_del         | FAIL   |
+-------------------------------+--------+
| test_evictor_with_repeat_keys | FAIL   |
+-------------------------------+--------+
```

In this testing phase, the first issue was fixing names within our test file to match their code. The next was dealing with issues within their reset function. This meant we had to do each test individually. We tested separately for the LRU and FIFO evictors, but most of the evictor tests failed, which lead us to believe there may be an issue with how their evictor is implemented within their cache_lib.cc.

###  Aryeh and Connor
```
+-------------------------------+--------+
| Test Name                     | Status |
+-------------------------------+--------+
| test_set                      | FAIL   |
+-------------------------------+--------+
| test_get                      | FAIL   |
+-------------------------------+--------+
| test_reset                    | PASS   |
+-------------------------------+--------+
| test_no_key                   | FAIL   |
+-------------------------------+--------+
| test_changed_key              | FAIL   |
+-------------------------------+--------+
| test_changed_key_size         | FAIL   |
+-------------------------------+--------+
| test_evictor                  | FAIL   |
+-------------------------------+--------+
| test_space_used               | PASS   |
+-------------------------------+--------+
| test_word_exceed              | FAIL   |
+-------------------------------+--------+
| test_evictor_twice            | FAIL   |
+-------------------------------+--------+
| test_evictor_empty            | FAIL   |
+-------------------------------+--------+
| test_evictor_with_del         | FAIL   |
+-------------------------------+--------+
| test_evictor_with_repeat_keys | FAIL   |
+-------------------------------+--------+
```
 Once we got to the evictor tests, there were some infinite loops that caused a lack of output. During this round of testing, we questioned the quality of our tests, especially our evictor tests, and put us in a position to reconsider them before we begin HW4.

### Hannah and Danny

```
+-------------------------------+--------+
| Test Name                     | Status |
+-------------------------------+--------+
| test_set                      | FAIL   |
+-------------------------------+--------+
| test_get                      | FAIL   |
+-------------------------------+--------+
| test_reset                    | PASS   |
+-------------------------------+--------+
| test_no_key                   | FAIL   |
+-------------------------------+--------+
| test_changed_key              | FAIL   |
+-------------------------------+--------+
| test_changed_key_size         | PASS   |
+-------------------------------+--------+
| test_evictor                  | FAIL   |
+-------------------------------+--------+
| test_space_used               | PASS   |
+-------------------------------+--------+
| test_word_exceed              | FAIL   |
+-------------------------------+--------+
| test_evictor_twice            | FAIL   |
+-------------------------------+--------+
| test_evictor_empty            | FAIL   |
+-------------------------------+--------+
| test_evictor_with_del         | FAIL   |
+-------------------------------+--------+
| test_evictor_with_repeat_keys | PASS   |
+-------------------------------+--------+
```

 Similar to the first two, we had to change some names around, but no compilation errors beyond that.This round of tests was more of the same. It informed us that some of the tests we had questions about were at least functional on someone else's code (because some of the evictor tests passed for this round). 

## Summary

All three projects we tested had an implementation of the LRU and FIFO evictors. When we switched them out, they had the same level of pass/fail. This leads us to the conclusion that the issue is in the use of evictor objects in the Cache_lib file as opposed to actual issues within the evictors themselves.

