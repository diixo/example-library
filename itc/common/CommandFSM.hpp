#ifndef AUTOFSM_COMMAND_HPP
#define AUTOFSM_COMMAND_HPP

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <typeinfo>
#include <memory>

// --------------------------------------------------------------------------
// User space
// --------------------------------------------------------------------------


/*******************************************************************/
/*                                                                 */
/* This is an auto-generated file for the finite-state machine.    */
/* Do not modify manually.                                         */
/*                                                                 */
/*******************************************************************/
 
// --------------------------------------------------------------------------
// Code definition
// --------------------------------------------------------------------------

class ICommandActionHandler;
class CCommandFSM
{
public:
	struct data_model {
		friend class CCommandFSM;
		data_model( void ): isAborted() { /* nop */ }
		bool isAborted;
	private:
		ICommandActionHandler* actionHandler;
	} model;

public:
	// --------------------------------------------------------------------------
	void setIsAborted( const bool& value )
	// --------------------------------------------------------------------------
	{
		model.isAborted = value;
	}

public:
	class state
	{
	public:
		virtual state* trigger_CANCEL( CCommandFSM& ) { return 0; }
		virtual state* trigger_FINISH( CCommandFSM& ) { return 0; }
		virtual state* trigger_FINISHED( CCommandFSM& ) { return 0; }
		virtual state* trigger_PAUSE( CCommandFSM& ) { return 0; }
		virtual state* trigger_RESUME( CCommandFSM& ) { return 0; }
		virtual state* trigger_RUN( CCommandFSM& ) { return 0; }
		virtual state* unconditional( CCommandFSM& ) { return 0; }
		virtual state* unconditional_async( CCommandFSM& ) { return 0; }
		virtual state* initial( CCommandFSM& ) { return 0; }

		template<class T> void enter( data_model&, ... ) {}
		template<class T> void exit( data_model&, ... ) {}
		virtual void exit( data_model&, const std::type_info& ) {}
		virtual ~state( void ) {}
	};

	state *cur_state;
	typedef state* ( state::*event )( CCommandFSM& );

private: 
	class CPAsyncEventTransitionData
	{
	public:
		// --------------------------------------------------------------------------
		CPAsyncEventTransitionData( CCommandFSM& fsm, const event trigger ): mFSM( fsm ), mTrigger( trigger ) { /* none */ }
		// --------------------------------------------------------------------------

		// --------------------------------------------------------------------------
		inline void dispatch( void ) { mFSM.dispatch( mTrigger ); }
		// --------------------------------------------------------------------------

	private:
		CCommandFSM& mFSM;
		event mTrigger;
	};

	class CPAsyncTriggerInvoker 
	{
	public:
		// --------------------------------------------------------------------------
		CPAsyncTriggerInvoker( CCommandFSM& fsm ) : mFsm( fsm ) { /* none */ }
		// --------------------------------------------------------------------------

		// --------------------------------------------------------------------------
		inline state* makeAsyncCall( const event trigger ) {
		// --------------------------------------------------------------------------
			return 0;
		}

		// --------------------------------------------------------------------------
		inline state* makeAsyncUCall( const event trigger ) {
		// --------------------------------------------------------------------------
			return (state*)0xFFFFFF;
		}

	protected:
		// --------------------------------------------------------------------------
		inline virtual void processEvent(  ) {  }
		// --------------------------------------------------------------------------

	private:
		CCommandFSM& mFsm;
	} asyncTriggerInvoker;

public:
	template<class C> class state_actions
	{
	protected:
		void inline enter( data_model& ) { /* default enter action */ }
		void inline exit( data_model& ) { /* default exit action */ }
	};

	template<class C, class P> class composite: public P, public state_actions<C>
	{
		virtual state* initial( CCommandFSM& ) { return 0; }
	public:
		// LCA calculation
		template<class T> void enter( data_model&, composite* ) {}
		template<class T> void enter( data_model &m, ... ) { P::template enter<T>( m, (T*)0 ); state_actions<C>::enter( m ); }
		template<class T> void exit( data_model&, composite* ) {}
		template<class T> void exit( data_model &m, ... ) { state_actions<C>::exit( m ); P::template exit<T>( m, (T*)0 ); }
		virtual void exit( data_model &m, const std::type_info &sti ) { if ( typeid(C) == sti ) return; state_actions<C>::exit( m ); P::exit( m, sti ); }
	};

	class no_state { /* dup */ };
	enum transition_type { external, internal };

	template<int index, event E, class S, class D0 = no_state> class transition_actions
	{
	protected:
		void enter(data_model&) { /* default enter action */ }
		bool condition(data_model&) { return true; } /* default condition action */
	};

	// external transition
	template<int index, event E, class S, class D = no_state, transition_type T = external> class transition: public transition_actions<index, E, S, D>
	{
	public:
		state* operator()( S *s, D &d, CCommandFSM &sc )
		{
			if( !transition_actions<index, E, S, D>::condition(sc.model) ) return 0;
			s->exit( sc.model, typeid(S) );
			s->template exit<D>( sc.model );
			transition_actions<index, E, S, D>::enter( sc.model );
			d.template enter<S>( sc.model );
			return &d;
		}
	};

	// internal transition
	template<int index, event E, class S, class D> class transition<index, E, S, D, internal>: public transition_actions<index, E, S, D>
	{
	public:
		state* operator()( S *s, D &d, CCommandFSM &sc )
		{
			if ( !transition_actions<index, E, S, D>::condition(sc.model) ) return 0;
			s->exit( sc.model, typeid(S) );
			s->template exit<D>( sc.model, (D*)0 );
			transition_actions<index, E, S, D>::enter( sc.model );
			d.template enter<S>( sc.model, (S*)0 );
			return &d;
		}
	};

	// transition with no target
	template<int index, event E, class S> class transition<index, E, S, no_state>: public transition_actions<index, E, S, no_state>
	{
	public:
		S* operator()( S *s, CCommandFSM &sc )
		{
			if ( !transition_actions<index, E, S, no_state>::condition(sc.model) ) return 0;
			transition_actions<index, E, S, no_state>::enter( sc.model );
			return s;
		}
	};

private:
	// --------------------------------------------------------------------------
	bool dispatch_event( event e )
	// --------------------------------------------------------------------------
	{
		state *next_state= (cur_state->*e)(*this);
		if ( next_state ) cur_state = next_state;
		return next_state != 0;
	}
	// --------------------------------------------------------------------------
	bool dispatch_uasync( event e )
	// --------------------------------------------------------------------------
	{
		return (cur_state->*e)(*this) == (state*)0xFFFFFF;
	}

public:
	// --------------------------------------------------------------------------
	void dispatch( event e )
	// --------------------------------------------------------------------------
	{
		bool cont = dispatch_event( e );
		while ( cont ) {
			if ( (cont = dispatch_event(&state::initial)) );
			else if ( dispatch_uasync(&state::unconditional_async) );
			else if ( cont = dispatch_event(&state::unconditional) );
			else break;
		}
	}

	// --------------------------------------------------------------------------
	CCommandFSM( ICommandActionHandler* pActionHandler ) : cur_state( &m_scxml ), asyncTriggerInvoker( *this )
	// --------------------------------------------------------------------------
	{
		model.actionHandler = pActionHandler;
	}

	// --------------------------------------------------------------------------
	void init( void )
	// --------------------------------------------------------------------------
	{
		dispatch( &state::initial );
	}

	class scxml : public composite<scxml, state>
	{
		// --------------------------------------------------------------------------
		state* initial( CCommandFSM&sc ) {
		// --------------------------------------------------------------------------
			return transition<0, &state::initial, scxml, state_Init, internal>()( this, sc.m_state_Init, sc ); }
		} m_scxml;

	class state_Init: public composite<state_Init, scxml>
	{
		// --------------------------------------------------------------------------
		inline virtual state* unconditional( CCommandFSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<1, &state::unconditional, state_Init, state_Idle>()( this, sc.m_state_Idle, sc );
			else return 0;
		}
	} m_state_Init;

	class state_Running: public composite<state_Running, scxml>
	{
		// --------------------------------------------------------------------------
		inline state* trigger_CANCEL( CCommandFSM &sc ) {
		// --------------------------------------------------------------------------
			bool& isAborted = sc.model.isAborted;
			if ( true, isAborted = true ) return transition<2, &state::trigger_CANCEL, state_Running, state_Aborting>()( this, sc.m_state_Aborting, sc );
			else return 0;
		}
		// --------------------------------------------------------------------------
		inline state* trigger_FINISH( CCommandFSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<3, &state::trigger_FINISH, state_Running, state_postProcessing>()( this, sc.m_state_postProcessing, sc );
			else return 0;
		}
		// --------------------------------------------------------------------------
		inline state* trigger_PAUSE( CCommandFSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<4, &state::trigger_PAUSE, state_Running, state_Paused>()( this, sc.m_state_Paused, sc );
			else return 0;
		}
		virtual state* unconditional( CCommandFSM &sc ) { return 0; }
	} m_state_Running;

	class state_Finished: public composite<state_Finished, scxml>
	{
	} m_state_Finished;

	class state_Aborting: public composite<state_Aborting, scxml>
	{
		// --------------------------------------------------------------------------
		inline state* trigger_FINISH( CCommandFSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<5, &state::trigger_FINISH, state_Aborting, state_postProcessing>()( this, sc.m_state_postProcessing, sc );
			else return 0;
		}
		// --------------------------------------------------------------------------
		inline state* trigger_PAUSE( CCommandFSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<6, &state::trigger_PAUSE, state_Aborting, state_Paused>()( this, sc.m_state_Paused, sc );
			else return 0;
		}
		virtual state* unconditional( CCommandFSM &sc ) { return 0; }
	} m_state_Aborting;

	class state_Idle: public composite<state_Idle, scxml>
	{
		// --------------------------------------------------------------------------
		inline state* trigger_CANCEL( CCommandFSM &sc ) {
		// --------------------------------------------------------------------------
			bool& isAborted = sc.model.isAborted;
			if ( true, isAborted = true ) return transition<8, &state::trigger_CANCEL, state_Idle, state_Canceling>()( this, sc.m_state_Canceling, sc );
			else return 0;
		}
		// --------------------------------------------------------------------------
		inline state* trigger_RUN( CCommandFSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<7, &state::trigger_RUN, state_Idle, state_Running>()( this, sc.m_state_Running, sc );
			else return 0;
		}
		virtual state* unconditional( CCommandFSM &sc ) { return 0; }
	} m_state_Idle;

	class state_Canceling: public composite<state_Canceling, scxml>
	{
		// --------------------------------------------------------------------------
		inline state* trigger_FINISH( CCommandFSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<9, &state::trigger_FINISH, state_Canceling, state_postProcessing>()( this, sc.m_state_postProcessing, sc );
			else return 0;
		}
		virtual state* unconditional( CCommandFSM &sc ) { return 0; }
	} m_state_Canceling;

	class state_Paused: public composite<state_Paused, scxml>
	{
		// --------------------------------------------------------------------------
		inline state* trigger_RESUME( CCommandFSM &sc ) {
		// --------------------------------------------------------------------------
			bool& isAborted = sc.model.isAborted;
			if ( !isAborted ) return transition<10, &state::trigger_RESUME, state_Paused, state_Running>()( this, sc.m_state_Running, sc );
			if ( isAborted ) return transition<11, &state::trigger_RESUME, state_Paused, state_postProcessing>()( this, sc.m_state_postProcessing, sc );
			else return 0;
		}
		virtual state* unconditional( CCommandFSM &sc ) { return 0; }
	} m_state_Paused;

	class state_postProcessing: public composite<state_postProcessing, scxml>
	{
		// --------------------------------------------------------------------------
		inline state* trigger_FINISHED( CCommandFSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<12, &state::trigger_FINISHED, state_postProcessing, state_Finished>()( this, sc.m_state_Finished, sc );
			else return 0;
		}
		virtual state* unconditional( CCommandFSM &sc ) { return 0; }
	} m_state_postProcessing;

};

class ICommandActionHandler
{
public:
	/* state handlers */
	virtual void stateInitEnter( CCommandFSM::data_model &m ) = 0;
	virtual void stateInitExit( CCommandFSM::data_model &m ) = 0;
	virtual void stateRunningEnter( CCommandFSM::data_model &m ) = 0;
	virtual void stateRunningExit( CCommandFSM::data_model &m ) = 0;
	virtual void stateFinishedEnter( CCommandFSM::data_model &m ) = 0;
	virtual void stateFinishedExit( CCommandFSM::data_model &m ) = 0;
	virtual void stateAbortingEnter( CCommandFSM::data_model &m ) = 0;
	virtual void stateAbortingExit( CCommandFSM::data_model &m ) = 0;
	virtual void stateIdleEnter( CCommandFSM::data_model &m ) = 0;
	virtual void stateIdleExit( CCommandFSM::data_model &m ) = 0;
	virtual void stateCancelingEnter( CCommandFSM::data_model &m ) = 0;
	virtual void stateCancelingExit( CCommandFSM::data_model &m ) = 0;
	virtual void statePausedEnter( CCommandFSM::data_model &m ) = 0;
	virtual void statePausedExit( CCommandFSM::data_model &m ) = 0;
	virtual void statePostProcessingEnter( CCommandFSM::data_model &m ) = 0;
	virtual void statePostProcessingExit( CCommandFSM::data_model &m ) = 0;

	/* transition handlers */
	virtual void onReset( CCommandFSM::data_model& m ) = 0;

protected:
	virtual ~ICommandActionHandler( void ) { /* nop */ }
};

class CCommandActionHandlerDefault: public ICommandActionHandler
{
public:
	/* state handlers */
	virtual void stateInitEnter( CCommandFSM::data_model &m ) { /* default */ }
	virtual void stateInitExit( CCommandFSM::data_model &m ) { /* default */ }
	virtual void stateRunningEnter( CCommandFSM::data_model &m ) { /* default */ }
	virtual void stateRunningExit( CCommandFSM::data_model &m ) { /* default */ }
	virtual void stateFinishedEnter( CCommandFSM::data_model &m ) { /* default */ }
	virtual void stateFinishedExit( CCommandFSM::data_model &m ) { /* default */ }
	virtual void stateAbortingEnter( CCommandFSM::data_model &m ) { /* default */ }
	virtual void stateAbortingExit( CCommandFSM::data_model &m ) { /* default */ }
	virtual void stateIdleEnter( CCommandFSM::data_model &m ) { /* default */ }
	virtual void stateIdleExit( CCommandFSM::data_model &m ) { /* default */ }
	virtual void stateCancelingEnter( CCommandFSM::data_model &m ) { /* default */ }
	virtual void stateCancelingExit( CCommandFSM::data_model &m ) { /* default */ }
	virtual void statePausedEnter( CCommandFSM::data_model &m ) { /* default */ }
	virtual void statePausedExit( CCommandFSM::data_model &m ) { /* default */ }
	virtual void statePostProcessingEnter( CCommandFSM::data_model &m ) { /* default */ }
	virtual void statePostProcessingExit( CCommandFSM::data_model &m ) { /* default */ }

	/* transition handlers */
	virtual void onReset( CCommandFSM::data_model& m ) { /* default */ }

protected:
	virtual ~CCommandActionHandlerDefault( void ) { /* nop */ }
};


// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Init>::enter( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Init>::exit( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Running>::enter( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateRunningEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Running>::exit( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateRunningExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Finished>::enter( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateFinishedEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Finished>::exit( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateFinishedExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Aborting>::enter( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateAbortingEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Aborting>::exit( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateAbortingExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Idle>::enter( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateIdleEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Idle>::exit( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateIdleExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Canceling>::enter( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCancelingEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Canceling>::exit( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCancelingExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Paused>::enter( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->statePausedEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_Paused>::exit( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->statePausedExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::transition_actions<10, &CCommandFSM::state::trigger_RESUME, CCommandFSM::state_Paused, CCommandFSM::state_Running>::enter(CCommandFSM::data_model& m )
// --------------------------------------------------------------------------
{
	m.actionHandler->onReset( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_postProcessing>::enter( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->statePostProcessingEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCommandFSM::state_actions<CCommandFSM::state_postProcessing>::exit( CCommandFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->statePostProcessingExit( m );
}

/* ************************************************************************ */
#endif // AUTOFSM_COMMAND_HPP
