/*
 * Copyright (c) 2019 Rajat and individual contributors.
 * License: https://github.com/xaivour/dawn/blob/master/LICENSE
 */

#pragma once

namespace dawn
{
/// Node in an intrusive linked list.
///
/// @ingroup Containers
struct ListNode
{
	ListNode* next;
	ListNode* prev;
};

namespace list
{
	inline void init_head(ListNode& head)
	{
		head.next = &head;
		head.prev = &head;
	}

	inline void insert(ListNode& node, ListNode& prev, ListNode& next)
	{
		// Kernel
		next.prev = &node;
		node.next = &next;
		node.prev = &prev;
		prev.next = &node;
	}

	inline void add(ListNode& node, ListNode& head)
	{
		insert(node, head, *head.next);
	}

	inline void remove(ListNode& node)
	{
		node.prev->next = node.next;
		node.next->prev = node.prev;
	}

} // namespace list

} // namespace dawn
