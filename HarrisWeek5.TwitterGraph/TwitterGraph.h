#pragma once

#include <vector>
#include <iostream>

#include "TwitterUser.h"
#include "Utility.h"

constexpr int USER_SIZE_PRINT_THRESHOLD = 5;

class TwitterGraph
{
	typedef TwitterUser Node;
	typedef std::vector<Node*> NodeList;

public:
	TwitterGraph() { }
	~TwitterGraph() { _users.~vector(); }
	
	void RemoveUser(Node* user)
	{
		std::vector<TwitterUser*>::iterator it = std::find(_users.begin(), _users.end(), user);

		if (it != _users.end())
		{
			(*it)->RemoveSelf();
			delete (*it);
			_users.erase(it);
		}
	}

	void Print()
	{
		std::cout << "==== TWITTER GRAPH ====\n";
		std::cout << "Number of users: " << _users.size() << "\n";
		std::cout << "Number of inactive users: " << GetInactiveUsers().size() << "\n";
		std::cout << "Number of isolated users: " << GetOrphanedUsers().size() << "\n\n";

		// only print the contents of the tree if it within a reasonable size.
		if (_users.size() <= USER_SIZE_PRINT_THRESHOLD)
		{
			for (Node* node : _users)
			{
				node->PrintContents();
			}
		}
	}

	/*
		Calculates the network distance (# of nodes) between two users.
		Returns -10 if there is no possible connection.
		Returns -1 if they is no found network connections.
		Returns 0 if they are in the same network.
		Returns > 0 for number of nodes in between the two users.
	*/
	int DistanceBetweenNodes(Node* first, Node* second)
	{
		if (_isUserIsolated(first) || _isUserIsolated(second))
		{
			return -10;
		}
		else if (first->CheckIsSubscribedTo(second) || second->CheckIsSubscribedTo(first))
		{
			return 0;
		}
		else
		{
			int distanceFromFirst = -1;
			NodeList fromFirstCache;

			bool foundConnection = _findConnectionBetweenNode(distanceFromFirst, fromFirstCache, first, second);

			// if no connection is found, return -1;
			if (!foundConnection)
			{
				return -1;
			}
			// if there was a connection, iterate through again starting from the second node to calculate the shortest path
			else
			{
				int distanceFromSecond = -1;
				NodeList fromSecondCache;

				_findConnectionBetweenNode(distanceFromSecond, fromSecondCache, second, first);

				if (distanceFromFirst == -1 && distanceFromSecond == -1)
				{
					return -1;
				}
				else
				{
					return (distanceFromFirst <= distanceFromSecond) ? distanceFromFirst : distanceFromSecond;
				}
			}
		}
	}

	int GetNumberOfUsers() const { return _users.size(); }

	Node* AddUser()
	{
		Node* ptr = new Node();
		_users.push_back(ptr);
		return ptr;
	}

	Node* AddUser(std::string name)
	{
		Node* ptr = new Node(name);
		_users.push_back(ptr);
		return ptr;
	}

	/* O(n) complex expression to find a node with the given name. */
	Node* FindUser(std::string name)
	{
		Node* ptr = NULL;

		for (NodeList::const_iterator i = _users.begin(); i != _users.end(); ++i)
		{
			if ((*i)->GetName() == name)
			{
				ptr = *i;
			}
		}

		return ptr;
	}

	/* O(n) complex expression to find a node with the given id. */
	Node* FindUser(int id)
	{
		Node* ptr = NULL;

		for (NodeList::const_iterator i = _users.begin(); i != _users.end(); ++i)
		{
			if ((*i)->GetID() == id)
			{
				ptr = *i;
			}
		}

		return ptr;
	}

	NodeList GetInactiveUsers()
	{
		NodeList nodes;

		for (Node* node : _users)
		{
			if (node->GetSubscriptions().size() <= 0)
			{
				nodes.push_back(node);
			}
		}

		return nodes;
	}

	NodeList GetOrphanedUsers()
	{
		NodeList nodes;

		for (Node* node : GetInactiveUsers())
		{
			if (node->GetFollowers().size() <= 0)
			{
				nodes.push_back(node);
			}
		}

		return nodes;
	}

	const NodeList& GetUsers() { return _users; }

private:
	NodeList _users;

	bool _findConnectionBetweenNode(int& refCount, NodeList& listOfNodesChecked, Node* first, Node* second)
	{
		bool isSubscribed = first->CheckIsSubscribedTo(second) || second->CheckIsSubscribedTo(first);

		if (isSubscribed)
		{
			refCount++;
			return true;
		}

		// the refCount needs to be set at 0 as any work after this point means that the end node is not in the same network as the root first
		// and refCount = 0 means they are.
		if (refCount <= 0)
		{
			refCount++;
		}

		NodeList subscriptions = first->GetSubscriptions();
		NodeList followers = first->GetFollowers();

		// check each subscription and see if they're subscribed to the searched node.
		for (Node* node : subscriptions)
		{
			if (node->CheckIsSubscribedTo(second))
			{
				refCount++;
				return true;
			}
		}

		// if the searched node is not found through the subscription path, check the follower path.
		for (Node* node : followers)
		{
			if (node->CheckIsSubscribedTo(second))
			{
				refCount++;
				return true;
			}
		}

		// this node (first) has been checked, now check children
		listOfNodesChecked.push_back(first);

		// now recursively check each child in subscriptions.
		for (Node* node : subscriptions)
		{
			// if the current node has not been checked already
			if (std::find(listOfNodesChecked.begin(), listOfNodesChecked.end(), node) == listOfNodesChecked.end())
			{
				// then recursively check it and return true if it found a connection
				if (_findConnectionBetweenNode(refCount, listOfNodesChecked, node, second))
				{
					refCount++;
					return true;
				}
			}
		}

		// if it wasn't found in subscriptions, check followers
		for (Node* node : followers)
		{
			if (std::find(listOfNodesChecked.begin(), listOfNodesChecked.end(), node) == listOfNodesChecked.end())
			{
				if (_findConnectionBetweenNode(refCount, listOfNodesChecked, node, second))
				{
					refCount++;
					return true;
				}
			}
		}

		// if this is reached, then no connection is found anywhere
		return false;
	}

	bool _isUserIsolated(TwitterUser*& user)
	{
		return user->GetFollowers().size() <= 0 && user->GetSubscriptions().size() <= 0;
	}
};
