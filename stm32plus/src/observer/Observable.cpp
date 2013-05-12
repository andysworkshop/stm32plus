/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/observer.h"


namespace stm32plus {

	/**
	 * Constructor
	 */

	Observable::Observable() {
		_first=nullptr;
	}

	/**
	 * Destructor. Destroy the links
	 */

	Observable::~Observable() {
		if(_first)
			_first->destroy();
	}

	/**
	 * Get the first link in the chain
	 * @return The first link.
	 */

	ObserverLink *Observable::getFirstObserverLink() {
		return _first;
	}

	/**
	 * Set the first link in the chain.
	 * @param[in] newFirst The new first link in the chain
	 */

	void Observable::setFirstObserverLink(ObserverLink* newFirst) {
		_first=newFirst;
	}

	/**
	 * Insert a new observer at the head of the list. A new link will be created to hold this observer.
	 * @param[in] newObserver The new observer to become the first in the list.
	 * @return The new observer link.
	 */

	ObserverLink *Observable::insertObserver(Observer& newObserver) {

		ObserverLink *newLink;

		newLink=new ObserverLink(*this,newObserver);
		newLink->setNext(_first);

		if(_first)
			_first->setPrevious(newLink);

		_first=newLink;
		return newLink;
	}

	/**
	 * Notify the observers of an event.
	 * @param[in] event The event being notified
	 * @param[in] context Some context to the event, the type of which will depend on the event_ parameter.
	 */

	void Observable::notifyObservers(ObservableEvent::E event,void *context) {

		ObserverLink *link;

		for(link=_first;link;link=link->getNext())
			link->getObserver().onNotify(*this,event,context);
	}
}
