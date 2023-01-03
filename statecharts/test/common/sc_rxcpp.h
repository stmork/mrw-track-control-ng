/* *
//
// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//
* */

#ifndef SC_RXCPP_H_
#define SC_RXCPP_H_

#include "sc_types.h"

namespace sc
{
	namespace rx
	{

		template<class T> class Observer;
		template<> class Observer<void> ;
		template<class T> struct subscription;
		template<class T> class Observable;
		template<> class Observable<void> ;

		template<class T>
		class Observer
		{
		public:
			virtual ~Observer()
			{
			}
			virtual void next(T value) = 0;
		};

		template<>
		class Observer<void>
		{
		public:
			virtual ~Observer()
			{
			}
			virtual void next() = 0;
		};

		template<class T>
		class SingleSubscriptionObserver: public Observer<T>
		{
		public:
			SingleSubscriptionObserver() :
				subscription(this)
			{
			}

			bool subscribe(sc::rx::Observable<T> * o)
			{
				return o->subscribe(&subscription);
			}

			bool unsubscribe(sc::rx::Observable<T> * o)
			{
				return o->unsubscribe(&subscription);
			}

		protected:
			sc::rx::subscription<T> subscription;
		};

		template<class T>
		struct subscription
		{

			subscription(Observer<T> * o) :
				observer(o), next(nullptr)
			{
			}

			Observer<T> * observer;
			subscription<T> * next;
		};

		template<class T> class ObservableBase
		{
		public:
			ObservableBase() :
				subscriptions(nullptr)
			{
			}

			bool subscribe(subscription<T> * s)
			{
				if (s != nullptr && s->observer != nullptr && s->next == nullptr)
				{
					subscription<T> * currentSub = this->subscriptions;
					s->next = (currentSub != nullptr) ? currentSub : s;
					this->subscriptions = s;
					return true;
				}
				return false;
			}

			bool unsubscribe(subscription<T> * s)
			{
				if (s != nullptr && this->subscriptions != nullptr)
				{
					if (this->subscriptions == s)
					{
						this->subscriptions = (s->next != s) ? s->next : nullptr;
						s->next = nullptr;

						return true;
					}

					sc::rx::subscription<T> * sub = this->subscriptions;
					while (sub != nullptr)
					{
						if (sub->next != sub && sub->next == s)
						{
							sub->next = (s->next != s) ? s->next : sub;
							return true;
						}

						sub = (sub->next != sub) ? sub->next : nullptr;
					}
				}
				return false;
			}

		protected:
			subscription<T> * subscriptions;

		};

		template<class T>
		class Observable: public ObservableBase<T>
		{

		public:

			void next(T value)
			{
				subscription<T> * sub = this->subscriptions;
				while (sub != nullptr)
				{
					if (sub->observer != nullptr)
					{
						sub->observer->next(value);
					}
					sub = (sub->next != sub) ? sub->next : nullptr;
				}
			}

		};

		template<>
		class Observable<void> : public ObservableBase<void>
		{

		public:
			void next()
			{
				subscription<void> * sub = this->subscriptions;
				while (sub != nullptr)
				{
					if (sub->observer != nullptr)
					{
						sub->observer->next();
					}
					sub = (sub->next != sub) ? sub->next : nullptr;
				}
			}

		};

	} /* namespace sc::rx */
} /* namespace sc */

#endif /* SC_RXCPP_H_ */
