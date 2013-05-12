/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/observer.h"


namespace stm32plus {

	/**
	 * Constructor. Set up the internal references.
	 * @param[in] base
	 *   The Observable that holds the start of this chain
	 * @param[in] observer
	 *   The observer to store in this link
	 */

	ObserverLink::ObserverLink(Observable& base,Observer& observer) :
		_base(base), _observer(observer) {

		_next=_previous=nullptr;
	}

	/**
	 * Self-destruct, call forwards in the chain then self-destruct ourselves.
	 */

	void ObserverLink::destroy() {
		if(_next)
			_next->destroy();

		_observer.onSubjectDestroyed();
		delete this;
	}

	/**
	 * Remove this link from the chain and self-destruct
	 */

	void ObserverLink::remove() {

		if(_previous == nullptr)
			_base.setFirstObserverLink(_next);
		else
			_previous->setNext(_next);

		delete this;
	}

	/**
	 * Set the next link in the chain to be the value.
	 * @param[in] newNext The new value for the next.
	 */

	void ObserverLink::setNext(ObserverLink* newNext) {
		_next=newNext;
	}

	/**
	 * Set the previous link in the chain to be the value.
	 * @param[in] newPrevious The new value for the previous.
	 */

	void ObserverLink::setPrevious(ObserverLink* newPrevious) {
		_previous=newPrevious;
	}

	/**
	 * Get the next link in the chain
	 * @return the next link
	 */

	ObserverLink* ObserverLink::getNext() {
		return _next;
	}

	/**
	 * Get the observer from this link
	 * @return The observer.
	 */

	Observer& ObserverLink::getObserver() {
		return _observer;
	}

}
