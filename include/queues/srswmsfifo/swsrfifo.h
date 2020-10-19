#ifndef SWSRFIFO_H_INCLUDED
#define SWSRFIFO_H_INCLUDED

template <typename T>
class SWSRFIFO {
private:
  struct Node {
    Node* next_;
    T     value_;
  };

#if FREELIST
  Node* freelisthead_;
  Node* freelisttail_;

  // this operation is practically identical as push_back() below
  inline void add_free_list_node(Node* free_node) {
    free_node->next_     = nullptr;
    freelisttail_->next_ = free_node;
    freelisttail_        = free_node;
  }

  // this operation is practically identical to pop_front() below
  inline Node* get_free_list_node() {
    auto node     = freelisthead_;
    auto new_head = node->next_;

    if (new_head==nullptr) {
      // freelist is empty, allocate new node
      return new Node;
    }

    freelisthead_ = new_head;
    return node;
  }

#else

  inline void add_free_list_node(Node* free_node) {
    delete free_node;
  }

  inline Node* get_free_list_node() {
    return new Node;
  }

#endif

  Node* head_;
  Node* tail_;

  SWSRFIFO(const SWSRFIFO&) = delete;
  SWSRFIFO& operator=(const SWSRFIFO&) = delete;

public:

  SWSRFIFO(
#if FREELIST
    int initFreeNodeSize = 0
#endif
  ) {    // initialize the fifo
    auto node    = new Node;
    node->next_   = nullptr;
    head_ = tail_ = node;

#if FREELIST
    // initilize the free list
    node          = new Node;
    node->next_   = nullptr;
    freelisthead_ = freelisttail_ = node;

    // pre-create freelist nodes
    for (int i=0; i<initFreeNodeSize; ++i) {
      add_free_list_node(new Node);
    }
#endif

  }

  ~SWSRFIFO() {
    Node* node;
    while (head_) {
      node  = head_;
      head_ = head_->next_;
      delete node;
    }

#if FREELIST
    while (freelisthead_) {
      node = freelisthead_;
      freelisthead_ = freelisthead_->next_;
      delete node;
    }
#endif

  }

  // push back updates tail_->next_ to point to the new node
  // then updates tail_ to point to the same new node.
  void push_back(const T& value) {

    auto new_node    = get_free_list_node();
    new_node->value_ = value;
    new_node->next_  = nullptr;

    // in case the list is empty, adding the first node will update
    // tail_->next_ to point to the new node. since head_ is also pointing
    // to the same dummy node in case of an empty list, this is the only
    // part where the push comes into potential conflict with a pop.
    tail_->next_     = new_node;
    tail_            = new_node;
  }

  bool pop_front(T& value) {

    auto node     = head_;

    // get the value of the 2nd node (1st node is a dummy node)
    // in case a concurrent access to ->next_ by push_back() above happens,
    // if the read below is atomic, then assigning it to head below is fine
    auto new_head = node->next_;

    // if the 2nd node is a nullptr, it means the queue is empty
    if (new_head==nullptr) {
      return false;
    }

    value = new_head->value_;

    // the 2nd node becomes the new dummy node
    head_ = new_head;

    // we return the original dummy node to the freelist
    add_free_list_node(node);

    return true;
  }

  bool is_empty() const {
    return (head_->next_==nullptr);
  }

};

#endif
