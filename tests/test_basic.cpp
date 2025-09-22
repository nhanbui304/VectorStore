#include "doctest/doctest.h"
#include "src/VectorStore.h"
#include "src/TestHelper.h"

TEST_CASE("VectorStore basic operations")
{
    TestHelper::resetNextId();
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
    CHECK(store.getId(0) == 1);

    // 6. Clear
    store.clear();
    CHECK(store.size() == 0);
    CHECK(store.empty() == true);
}

TEST_CASE("VectorStore helper functions")
{
    TestHelper::resetNextId();
    VectorStore store(3, TestHelper::VOTIENEmbedding);
    store.setEmbeddingFunction(TestHelper::countCharsPerWord);

    store.addText("aA BC EE");

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
    TestHelper::resetNextId();
    VectorStore store(2, TestHelper::VOTIENEmbedding);

    store.addText("a b");
    store.addText("c d");

    std::ostringstream oss;
    std::streambuf *oldCoutBuf = std::cout.rdbuf(oss.rdbuf());
    store.forEach([](SinglyLinkedList<float> &vec, int id, string &raw)
                  { std::cout << id << ":" << raw << "=" << vec.toString() << "\n"; });
    std::cout.rdbuf(oldCoutBuf);

    std::string output = oss.str();
    CHECK(output == "0:a b=[1]->[2]\n1:c d=[3]->[4]\n");
}

TEST_CASE("VectorStore distance and similarity functions")
{
    TestHelper::resetNextId();
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
    TestHelper::resetNextId();
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
    TestHelper::resetNextId();
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
    TestHelper::resetNextId();
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

TEST_CASE("test_175")
{
    TestHelper::resetNextId();
    VectorStore store(2, TestHelper::encode);

    store.addText("10 0");
    store.addText("0 10");
    store.addText("7 7");

    auto *query = store.preprocessing("8 8");
    int *result = store.topKNearest(*query, 2, "euclidean");

    CHECK(store.getRawText(result[0]) == "7 7");
    CHECK(store.getRawText(result[1]) == "10 0");

    delete[] result;
    delete query;
}

TEST_CASE("test_179")
{
    TestHelper::resetNextId();
    VectorStore store(2, TestHelper::encode);

    store.addText("1 0");
    store.addText("0 1");
    store.addText("1 1");
    store.addText("2 2");
    store.addText("3 3");

    auto *query = store.preprocessing("1 1");
    int *result = store.topKNearest(*query, 4, "cosine");

    CHECK(store.getRawText(result[0]) == "1 1");
    CHECK(store.getRawText(result[1]) == "2 2");
    CHECK(store.getRawText(result[2]) == "3 3");
    CHECK(store.getRawText(result[3]) == "1 0");

    delete[] result;
    delete query;
}

TEST_CASE("test_208")
{
    TestHelper::resetNextId();
    VectorStore store(28 * 28, TestHelper::readMNISTLine);
    auto lines = TestHelper::readMNISTFile("tests/mnist.csv");
    vector<int> labels;
    int trainSize = 120;
    for (int i = 0; i < trainSize; i++)
    {
        store.addText(lines[i].substr(2));
        labels.push_back(lines[i][0] - '0');
    }

    auto *query = TestHelper::readMNISTLine(lines[50].substr(2));
    int k = 6;
    int *nearestIndices = store.topKNearest(*query, k, "euclidean");
    string result = "";
    for (int i = 0; i < k; i++)
        result += to_string(nearestIndices[i] + 2) + ":" + to_string(labels[nearestIndices[i]]) + " ";

    CHECK(result == "52:3 100:3 12:3 76:3 20:6 50:9 ");
    delete[] nearestIndices;
    delete query;
}

TEST_CASE("test_231")
{
    TestHelper::resetNextId();
    VectorStore store(28 * 28, TestHelper::readMNISTLine);
    auto lines = TestHelper::readMNISTFile("tests/mnist.csv");
    vector<int> labels;
    int trainSize = 45;
    for (int i = 0; i < trainSize; i++)
    {
        store.addText(lines[i].substr(2));
        labels.push_back(lines[i][0] - '0');
    }

    string results = "";
    for (size_t i = trainSize; i < lines.size() - 130; i++)
    {
        auto *query = TestHelper::readMNISTLine(lines[i].substr(2));
        int nearestIndex = store.findNearest(*query, "manhattan");
        results += to_string(labels[nearestIndex]) + " ";
        delete query;
    }

    CHECK(results == "9 1 5 1 3 3 0 7 9 4 8 0 9 4 1 7 1 6 0 6 5 6 1 5 ");
}