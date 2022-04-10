
#pragma once

#include <functional>
#include <memory>

namespace tuple_utils {

    template<int...> struct index_tuple{}; 

    template<int I, typename IndexTuple, typename... Types> 
    struct make_indexes_impl; 

    template<int I, int... Indexes, typename T, typename ... Types> 
    struct make_indexes_impl<I, index_tuple<Indexes...>, T, Types...> 
    { 
        typedef typename make_indexes_impl<I + 1, index_tuple<Indexes..., I>, Types...>::type type; 
    }; 

    template<int I, int... Indexes> 
    struct make_indexes_impl<I, index_tuple<Indexes...> > 
    { 
        typedef index_tuple<Indexes...> type; 
    }; 

    template<typename ... Types> 
    struct make_indexes : make_indexes_impl<0, index_tuple<>, Types...> 
    {}; 





    template<class Ret, class... Args, int... Indexes >
    Ret apply_helper(const std::function<Ret(Args...)>& pf, index_tuple< Indexes... >, std::tuple<Args...>&& tup) 
    {
        return pf( std::forward<Args>( std::get<Indexes>(tup))... ); 
    } 

    template<class Ret, class ... Args>
    Ret apply(const std::function<Ret(Args...)>& pf, const std::tuple<Args...>&  tup)
    {
        return apply_helper(pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
    }

    template<class Ret, class ... Args>
    Ret apply(const std::function<Ret(Args...)>& pf, std::tuple<Args...>&&  tup)
    {
        return apply_helper(pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
    }







    template<typename Context, class... Args, int... Indexes > 
    void apply_helper(std::shared_ptr<Context> context, std::function<void (Context*, Args...)> pf, index_tuple< Indexes... >, std::tuple<Args...>&& tup) 
    {
        pf(context.get(), std::forward<Args>( std::get<Indexes>(tup))...);
    } 

    template<typename Context, class ... Args> 
    void apply(std::shared_ptr<Context> context, std::function<void (Context*, Args...)> pf, const std::tuple<Args...>&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
    }

    template<typename Context, class ... Args>
    void apply(std::shared_ptr<Context> context, std::function<void (Context*, Args...)> pf, std::tuple<Args...>&&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
    }






    template<typename Context, typename Ret, class... Args, int... Indexes >
    Ret apply_helper(std::shared_ptr<Context> context, std::function<Ret (Context*, Args...)> pf, index_tuple< Indexes... >, std::tuple<Args...>&& tup) 
    {
        return pf(context.get(), std::forward<Args>( std::get<Indexes>(tup))...);
    } 

    template<typename Context, typename Ret, class ... Args>
    Ret apply(std::shared_ptr<Context> context, std::function<Ret (Context*, Args...)> pf, const std::tuple<Args...>&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::tuple<Args...>(tup));
    }

    template<typename Context, typename Ret, class ... Args>
    Ret apply(std::shared_ptr<Context> context, std::function<Ret (Context*, Args...)> pf, std::tuple<Args...>&&  tup)
    {
        return apply_helper(context, pf, typename make_indexes<Args...>::type(), std::forward<std::tuple<Args...>>(tup));
    }
}
