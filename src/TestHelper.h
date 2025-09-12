#ifndef TESTHELPER_H
#define TESTHELPER_H

#include "VectorStore.h"

class TestHelper
{
public:
    /**
     * @brief Simple character-based embedding function.
     *
     * This function converts a given string into a vector of floats, where each element
     * represents the sum of alphabetical positions of characters in each word.
     * Non-alphabetic characters are ignored. Words are separated by spaces.
     *
     * Alphabetical positions:
     *   'a' = 1 'b' = 2 ...'z' = 26
     *   'A' = -1 'B' = -2 ... 'Z' = -26
     *
     * Example:
     *   Input: "aA BC EE"
     *   Processing:
     *     "aA" -> 1 + -1 = 0
     *     "bc" -> 2 + 3 = 5
     *     "EE" -> -5 + -5 = -10
     *   Output vector: [0, 5, -10]
     *
     * @param text Input string to be embedded.
     * @return SinglyLinkedList<float>* Pointer to the dynamically allocated vector representation.
     */
    static SinglyLinkedList<float> *VOTIENEmbedding(const string &text)
    {
        SinglyLinkedList<float> *vec = new SinglyLinkedList<float>();
        stringstream ss(text);
        string word;

        while (ss >> word)
        {
            int sum = 0;
            for (char c : word)
            {
                if (c >= 'a' && c <= 'z')
                    sum += c - 'a' + 1;
                else if (c >= 'A' && c <= 'Z')
                    sum -= c - 'A' + 1;
            }
            vec->add(sum);
        }

        return vec;
    }

    /**
     * @brief Count the number of alphabetic characters in each word of the string.
     *
     * This function splits the input string by spaces, counts the number of alphabetic
     * characters in each word, and stores the counts in a vector.
     *
     * Example:
     *   Input: "aA BC EE!"
     *   Processing:
     *     "aA" -> 2 alphabetic chars
     *     "BC" -> 2 alphabetic chars
     *     "EE!" -> 2 alphabetic chars ('!' ignored)
     *   Output vector: [2, 2, 2]
     *
     * @param text Input string to count characters.
     * @return SinglyLinkedList<float>* Pointer to the dynamically allocated vector of counts.
     */
    static SinglyLinkedList<float> *countCharsPerWord(const string &text)
    {
        SinglyLinkedList<float> *vec = new SinglyLinkedList<float>();
        stringstream ss(text);
        string word;

        while (ss >> word)
        { // Split by spaces
            int count = 0;
            for (char c : word)
            {
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                {
                    count++;
                }
            }
            vec->add(count);
        }

        return vec;
    }

    /**
     * @brief Example action function to double all elements of a vector.
     *
     * This function can be passed to VectorStore::forEach to modify each stored vector.
     *
     * @param vec Reference to the vector of floats to modify.
     * @param id ID of the vector (not used in this example).
     * @param rawText Reference to the original text (not used in this example).
     */
    static void doubleVectorElements(SinglyLinkedList<float> &vec, int id, string &rawText)
    {
        for (int i = 0; i < vec.size(); i++)
        {
            float &value = vec.get(i);
            value = value * 2;
        }
    }

    static ArrayList<VectorStore::VectorRecord *> &getRecords(VectorStore &store)
    {
        return store.records;
    }

    static int getDimension(VectorStore &store)
    {
        return store.dimension;
    }

    static int getCount(VectorStore &store)
    {
        return store.count;
    }

    static VectorStore::EmbedFn getEmbeddingFunction(VectorStore &store)
    {
        return store.embeddingFunction;
    }
};

#endif // TESTHELPER_H
