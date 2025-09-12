#include "doctest/doctest.h"
#include "src/VectorStore.h"
#include "src/TestHelper.h"

TEST_CASE("VectorStore basic operations")
{
    VectorStore store(2, TestHelper::VOTIENEmbedding);
    CHECK(store.size() == 0);
    CHECK(store.empty() == true);

    store.addText("aA BC");
    CHECK(store.size() == 1);
    CHECK(store.empty() == false);
    CHECK(store.getRawText(0) == "aA BC");

    auto vecPtr = store.preprocessing("aA BC");
    REQUIRE(vecPtr != nullptr);
    CHECK(vecPtr->size() == 2);  // 2 words: "aA", "BC"
    CHECK(vecPtr->get(0) == 0);  // aA -> 1 - -1 = 0
    CHECK(vecPtr->get(1) == -5); // BC -> -2 + -3 = 5
    delete vecPtr;

    SinglyLinkedList<float> &v0 = store.getVector(0);
    CHECK(v0.toString() == "[0]->[-5]");

    bool removed = store.removeAt(0);
    CHECK(removed == true);
    CHECK(store.size() == 0);
    store.addText("Hello World");
    bool updated = store.updateText(0, "Test Text");
    CHECK(updated == true);
    CHECK(store.getRawText(0) == "Test Text");
    CHECK(store.getId(0) == 0);

    // 6. Clear
    store.clear();
    CHECK(store.size() == 0);
    CHECK(store.empty() == true);
}

TEST_CASE("VectorStore helper functions")
{
    VectorStore store(3, TestHelper::VOTIENEmbedding);
    store.setEmbeddingFunction(TestHelper::countCharsPerWord);

    CHECK(TestHelper::getDimension(store) == 3);
    CHECK(TestHelper::getCount(store) == 0);
    CHECK(TestHelper::getEmbeddingFunction(store) != nullptr);

    store.addText("aA BC EE");
    CHECK(TestHelper::getCount(store) == 1);
    auto &records = TestHelper::getRecords(store);
    REQUIRE(records.size() == 1);
    CHECK(records.get(0)->rawText == "aA BC EE");

    auto vec2 = store.preprocessing("aA BC EE");
    REQUIRE(vec2 != nullptr);
    CHECK(vec2->size() == 3);
    CHECK(vec2->get(0) == 2);
    CHECK(vec2->get(1) == 2);
    CHECK(vec2->get(2) == 2);
    delete vec2;
}

TEST_CASE("VectorStore forEach with doubleVectorElements")
{
    VectorStore store(3, TestHelper::VOTIENEmbedding);

    store.addText("aA BC");       // vector: [0, -5, 0]
    store.addText("EE EE EE EE"); // vector: [-10, -10, -10]

    store.forEach(TestHelper::doubleVectorElements);

    auto &v0 = store.getVector(0);
    CHECK(v0.toString() == "[0]->[-10]->[0]");

    auto &v1 = store.getVector(1);
    CHECK(v1.size() == 3);
    CHECK(v1.toString() == "[-20]->[-20]->[-20]");
}

TEST_CASE("VectorStore distance and similarity functions")
{
    VectorStore store(2, TestHelper::VOTIENEmbedding);

    SinglyLinkedList<float> v1;
    v1.add(1);
    v1.add(2);

    SinglyLinkedList<float> v2;
    v2.add(2);
    v2.add(3);

    double cosSim = store.cosineSimilarity(v1, v2);
    double expectedCos = (1 * 2 + 2 * 3) / (std::sqrt(1 * 1 + 2 * 2) * std::sqrt(2 * 2 + 3 * 3));
    CHECK(cosSim == doctest::Approx(expectedCos));

    // 2. L1 distance (Manhattan)
    double l1 = store.l1Distance(v1, v2);
    CHECK(l1 == 2); // |1-2| + |2-3| = 1+1=2

    // 3. L2 distance (Euclidean)
    double l2 = store.l2Distance(v1, v2);
    CHECK(l2 == doctest::Approx(std::sqrt(1 * 1 + 1 * 1))); // sqrt((1-2)^2 + (2-3)^2) = sqrt(2)
}

TEST_CASE("VectorStore findNearest with different metrics")
{
    VectorStore store(2, TestHelper::VOTIENEmbedding);

    // Thêm các vector
    store.addText("C c");
    store.addText("C C");
    store.addText("b A");
    store.addText("d b");
    store.addText("e d");

    const SinglyLinkedList<float> *query = store.preprocessing("aa b");
    CHECK(query->toString() == "[2]->[2]");

    int nearestCosine = store.findNearest(*query, "cosine");
    CHECK(nearestCosine == 4);

    int nearestManhattan = store.findNearest(*query, "manhattan");
    CHECK(nearestManhattan == 3);

    int nearestEuclidean = store.findNearest(*query, "euclidean");
    CHECK(nearestEuclidean == 3);

    delete query;
}

TEST_CASE("VectorStore findNearest with different metrics")
{
    VectorStore store(2, TestHelper::VOTIENEmbedding);

    // Thêm các vector
    store.addText("C c");
    store.addText("C C");
    store.addText("b A");
    store.addText("d b");
    store.addText("e d");

    const SinglyLinkedList<float> *query = store.preprocessing("aa b");
    CHECK(query->toString() == "[2]->[2]");

    int nearestCosine = store.findNearest(*query, "cosine");
    CHECK(store.getVector(nearestCosine).toString() == "[5]->[4]");

    int nearestManhattan = store.findNearest(*query, "manhattan");
    CHECK(store.getVector(nearestManhattan).toString() == "[4]->[2]");

    int nearestEuclidean = store.findNearest(*query, "euclidean");
    CHECK(store.getVector(nearestEuclidean).toString() == "[4]->[2]");

    delete query;
}

TEST_CASE("VectorStore topKNearest")
{
    VectorStore store(2, TestHelper::VOTIENEmbedding);

    // Thêm các vector
    store.addText("C c");
    store.addText("C C");
    store.addText("b A");
    store.addText("d b");
    store.addText("e d");

    const SinglyLinkedList<float> *query = store.preprocessing("aa b");
    CHECK(query->toString() == "[2]->[2]");

    int k = 3;
    int *topKCosine = store.topKNearest(*query, k, "euclidean");
    CHECK(store.getVector(topKCosine[0]).toString() == "[4]->[2]");
    CHECK(store.getVector(topKCosine[1]).toString() == "[2]->[-1]");
    CHECK(store.getVector(topKCosine[2]).toString() == "[5]->[4]");

    delete query;
    delete[] topKCosine;
}
