from __future__ import annotations
from turtle import Turtle, window_width, exitonclick, Screen
from dataclasses import dataclass

YH = 70

PREV_POS_STACK = []
OFFSETS = [480, 240, 120, 60, 30, 15]

@dataclass
class BSTNode:
    data: int
    left: 'BSTNode' = None
    right: 'BSTNode' = None

    @staticmethod
    def insert(node, data):
        if node == None:
            return BSTNode(data)
        
        if node.data >= data:
            node.left = BSTNode.insert(node.left, data)
        else:
            node.right = BSTNode.insert(node.right, data)

        return node


class BST:
    def __init__(self):
        self.root = None

    def insert(self, data):
        self.root = BSTNode.insert(self.root, data)

    def inorder(self, node = -1):
        if node == -1: node = self.root
        if not node:
            return []

        return self.inorder(node.left) + [node.data] + self.inorder(node.right)


def display_node(turtle, x, y, node):
    turtle.penup()
    turtle.goto(x, -y)
    turtle.write(node.data)
    print(x, y, node.data)
    turtle.pendown()
    turtle.circle(30)


def draw_node(turtle, node, is_left, level):
    if not node:
        return

    y = level * YH
    x = ((-1 if is_left else 1) * OFFSETS[level - 1]) + PREV_POS_STACK[-1];
    print(is_left, x, PREV_POS_STACK[-1])

    display_node(turtle, x, y, node)

    PREV_POS_STACK.append(x)

    draw_node(turtle, node.left, True, level + 1)
    draw_node(turtle, node.right, False, level + 1)

    PREV_POS_STACK.pop()

def display_bst(turtle, bst):
    x = 0
    y = 0

    display_node(turtle, x, y, bst.root)

    PREV_POS_STACK.append(x)

    draw_node(turtle, bst.root.left, True, 1)
    draw_node(turtle, bst.root.right, False, 1)

    PREV_POS_STACK.pop()


def get_hight(node):
    if node is None:
        return 0

    if node.left is None and node.right is None:
        return 1

    return 1 + max(get_hight(node.left), get_hight(node.right))


if __name__ == "__main__":
    bst = BST()

    bst.insert(181)
    bst.insert(200)
    bst.insert(154)
    bst.insert(208)
    bst.insert(147)
    bst.insert(204)
    bst.insert(206)
    bst.insert(203)
    bst.insert(205)
    bst.insert(207)
    bst.insert(190)
    bst.insert(195)
    bst.insert(185)
    bst.insert(259)
    bst.insert(159)

    print(get_hight(bst.root))

    screen = Screen()
    screen.setup(800, 300)
    # screen.setworldcoordinates(0, 0, 800, 300)
    print(bst.inorder())

    turtle = Turtle()
    maxx = window_width()
    print(maxx)
    display_bst(turtle, bst)

    screen.update()
    screen.exitonclick()
