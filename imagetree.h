#ifndef IMAGETREE_H
#define IMAGETREE_H

#include "polygon.h"

class ImageTree;

class Node
{
    friend ImageTree;

public:
    Node(Polygon polygon, Node* parent = nullptr, Node* leftChild = nullptr, Node* rightChild = nullptr);

private:
    Polygon mPolygon;

    Node* mParent;
    Node* mLeftChild;
    Node* mRightChild;
};

class ImageTree
{
public:
    explicit ImageTree(Node* head = nullptr);
    ~ImageTree();

    Node* getHead();
    unsigned int getPolygonsNumber() { _getPolygonsNumber(mHead); return mPolygonsNumber; }

    QImage toImage();
    QImage toImageGrid();

    void insert(Polygon polygon) { mHead = _insert(polygon, nullptr, mHead); }
    void clear() { mPolygonsNumber = 0; _clear(mHead); mHead = nullptr; }

private:
    Node* mHead;
    unsigned int mPolygonsNumber;

    void _getPolygonsNumber(Node* node);
    void _toImage(QImage& image, Node* node);
    void _toImageGrid(QImage& image, Node* node);

    Node* _insert(Polygon polygon, Node* parent = nullptr, Node* node = nullptr);
    void _clear(Node* node);

    bool isLeaf(Node* node);
    bool isLeftChild(Polygon& newPolygon, Polygon& currentPolygon);
};

#endif // IMAGETREE_H
