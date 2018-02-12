#ifndef  __BSL_VAR_MAGIC_DICT_H__
#define  __BSL_VAR_MAGIC_DICT_H__

#include <map>
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 3
#include <unordered_map>
#elif __GNUC__ >= 3
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#include "Lsc/pool/Lsc_pool.h"
#include "Lsc/var/IVar.h"
#include "Lsc/var/MagicRef.h"
#include "Lsc/var/Null.h"
#include "Lsc/var/utils.h"

#if __GNUC__ < 3
namespace __gnu_cxx{
    /**
    *  
    *  
    */
    using std::hash_map;
    using std::hash;
}
#endif

namespace Lsc{
namespace var{
    //forward declarations & typedefs
    template<typename implement_t>
        class MagicBasicDict;

    template<template<class>class Alloc>
        class __MagicStdMapAdapter;

    template<template<class>class Alloc>
        class __MagicGnuHashMapAdapter;

    /**
    *  
    */
    typedef MagicBasicDict<__MagicStdMapAdapter<Lsc::pool_allocator> >    MagicStdMapDict;

    /**
    *  
    *  
    */
    typedef MagicBasicDict<__MagicGnuHashMapAdapter<Lsc::pool_allocator> >MagicGnuHashDict;

    /**
    *  
    *  Ŀǰ���Ƽ�ʹ�õ��Ƿ�װ��std::map����ʹ��Lsc::pool_allocator��StdMapDict
    */
    typedef MagicStdMapDict                                          MagicDict;


    template<typename implement_t>
        class MagicBasicDict: plclic IVar{
        plclic:
            typedef IVar::string_type                       string_type;
            typedef IVar::field_type                        field_type;
            typedef typename implement_t::allocator_type    allocator_type;
            typedef DictIterator                            dict_iterator;
            typedef DictConstIterator                       dict_const_iterator;
            typedef MagicRef                                reference_type;

        plclic:
            MagicBasicDict(Lsc::ResourcePool& rp)
                :_dict(), _alloc(), _rp(rp) {}

            explicit MagicBasicDict( Lsc::ResourcePool& rp, size_t init_capacity, const allocator_type& alloc_ = allocator_type() )
                :_dict(init_capacity, alloc_), _alloc(alloc_), _rp(rp) {}

            explicit MagicBasicDict( Lsc::ResourcePool& rp, const allocator_type& alloc_ )
                :_dict(alloc_), _alloc(alloc_), _rp(rp) {}

            MagicBasicDict( const MagicBasicDict& other )
                :IVar(other), _dict(other._dict), _alloc(other._alloc), _rp(other._rp) {}

            MagicBasicDict& operator = ( const MagicBasicDict& other ){
                try{
                    _dict = other._dict;
                    _alloc= other._alloc;
                }catch(Lsc::Exception& e){
                    e<<"{"<<__PRETTY_FUNCTION__<<"("<<other.dump(0)<<")}";
                    throw;
                }catch(std::exception& e){
                    throw StdException(e)<<BSL_EARG<<"{"<<__PRETTY_FUNCTION__<<"("<<other.dump(0)<<")}";
                }catch(...){
                    throw UnknownException()<<BSL_EARG<<"{"<<__PRETTY_FUNCTION__<<"("<<other.dump(0)<<")}";
                }
                return *this;
            }

            virtual ~MagicBasicDict() {}

            /**
             *
            **/
            virtual size_t size() const {
                return _dict.size();
            }

            /**
             *
             **/
            virtual void clear(){
                return _dict.clear();
            }
            
            /**
             *
             * �ú���ֻ��¡�����㣬����¡�ӽ�㣬���������ͣ���¡ָ��Ľ��
             *
            **/
            virtual MagicBasicDict& clone( Lsc::ResourcePool& rp ) const {
                return rp.clone(*this);
            }

            /**
             * 
             * ��is_deep_copy���������Ƿ����
             * ��Ϊfalse, ֻ��¡�����㣬����¡�ӽ��
             * ��Ϊtrue, ��¡�����㣬���ҵݹ��¡�ӽ��
             * ���������ͣ���¡ָ��Ľ��
             * 
            **/
            virtual MagicBasicDict& clone( Lsc::ResourcePool& rp, bool is_deep_copy ) const {
                MagicBasicDict& res = rp.clone(*this);
                if(is_deep_copy){
                    IVar::dict_const_iterator iter = dict_begin();
                    IVar::dict_const_iterator end  = dict_end();
                    for(; iter != end ; ++iter){
                        res.set(iter->key(), (iter->value()).clone(rp, is_deep_copy));
                    }
                }
                return res;
            }

            /**
             *
            **/
            /**
             *
            **/
            virtual string_type dump(size_t verbose_level = 0) const {
                string_type res;
                if ( verbose_level == 0 ){
                    res.appendf("[Lsc::var::MagicBasicDict] this[%p] size[%zd]", this, _dict.size() ); 
                }else{
                    dump_to_string(*this, res, verbose_level, "", 0 );
                }
                return res;
            }

            /**
             *
            **/
            virtual string_type to_string() const {
                return dump(0);
            }

            virtual MagicBasicDict& operator = ( IVar& var ) {
                if ( typeid(var) == typeid(*this) ){
                    _dict = dynamic_cast<const MagicBasicDict&>(var)._dict;
                }else if ( var.is_dict() ){
                    _dict.clear();
                    IVar::dict_iterator iter_ = var.dict_begin();
                    IVar::dict_iterator end  = var.dict_end();
                    for( ; iter_ != end ; ++ iter_ ){
                        Lsc::var::MagicRef mr(_rp);
                        mr = iter_->value();
                        _dict.insert(std::make_pair(iter_->key(), mr));
                    }
                }else{
                    throw Lsc::InvalidOperationException()<<BSL_EARG<<"cast from "<<var.dump();
                }
                return *this;   
            }


            /**
             *
            **/
            virtual string_type get_type() const {
                return "Lsc::var::MagicBasicDict";
            }

            /**
             *
            **/
            virtual IVar::mask_type get_mask() const {
                return IVar::IS_DICT;
            }

            //testers
            virtual bool is_dict() const {
                return true;
            }

            //converters
            using IVar::operator =;

            //methods for dict
            virtual IVar& get( const field_type& name ) {
                iter_impl_t iter_ = _dict.find( name );
                if ( iter_ == _dict.end() ){
                    return Lsc::var::Null::null;
                }
                return iter_->second;
            }

            virtual IVar& get( const field_type& name, IVar& default_value ) {
                iter_impl_t iter_ = _dict.find( name );
                if ( iter_ == _dict.end() ){
                    return default_value;
                }
                return iter_->second;
            }

            virtual const IVar& get( const field_type& name ) const {
                const_iter_impl_t iter_ = _dict.find( name );
                if ( iter_ == _dict.end() ){
                    return Lsc::var::Null::null;
                }
                return iter_->second;
            }

            virtual const IVar& get( const field_type& name, const IVar& default_value ) const {
                const_iter_impl_t iter_ = _dict.find( name );
                if ( iter_ == _dict.end() ){
                    return default_value;
                }
                return iter_->second;
            }

            virtual void set( const field_type& name, IVar& value ){
                Lsc::var::MagicRef mr(_rp);
                mr = value;
                typedef std::pair<typename implement_t::iterator, bool> pair_t;
                pair_t res = _dict.insert( std::pair<field_type, Lsc::var::MagicRef>( name,  mr ) );
                if ( !res.second ){
                    //already exist
                    res.first->second = mr;
                }
            }

            virtual bool del( const field_type& name ) {
                iter_impl_t iter_ = _dict.find( name );
                if ( iter_ == _dict.end() ){
                    return false;
                }
                _dict.erase( iter_ );
                return true;
            }

            virtual dict_const_iterator dict_begin() const {
                return dict_const_iterator( _s_create_const_iterator( _dict.begin(), &_alloc ), _s_clone_const_iterator, _s_destroy_const_iterator, &_alloc );
            }

            virtual dict_iterator dict_begin() {
                return dict_iterator( _s_create_iterator( _dict.begin(), &_alloc ), _s_clone_iterator, _s_destroy_iterator, &_alloc );
            }

            virtual dict_const_iterator dict_end() const {
                return dict_const_iterator( _s_create_const_iterator( _dict.end(), &_alloc ), _s_clone_const_iterator, _s_destroy_const_iterator, &_alloc );
            }

            virtual dict_iterator dict_end() {
                return dict_iterator( _s_create_iterator( _dict.end(), &_alloc ), _s_clone_iterator, _s_destroy_iterator, &_alloc );
            }

            virtual const IVar& operator []( const field_type& name ) const {
                const_iter_impl_t iter_ = _dict.find(name);
                if ( iter_ == _dict.end() ){
                    throw Lsc::KeyNotFoundException()<<BSL_EARG<<"key["<<name<<"]";
                }
                return iter_->second;
            }

            virtual IVar& operator []( const field_type& name ){
                iter_impl_t iter_ = _dict.find(name);
                if ( iter_ == _dict.end() ){
                    iter_ = _dict.insert(std::make_pair(name, Lsc::var::MagicRef(_rp))).first;
                }
                return iter_->second;
            }

            //methods for array
#if __GNUC__ > 2
            using IVar::operator [];
            using IVar::get;
            using IVar::set;
            using IVar::del;
#else
            //avoid using bug of g++ 2.96
            virtual IVar& get( size_t idx ) {
                throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
            }

            virtual IVar& get( size_t idx, IVar& default_value ) {
                throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] default["<<default_value.dump(0)<<"]";
            }

            virtual const IVar& get( size_t idx ) const {
                throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
            }

            virtual const IVar& get( size_t idx, const IVar& default_value ) const {
                throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] default["<<default_value.dump(0)<<"]";
            }

            virtual void set( size_t idx, IVar& value_ ){
                throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"] value["<<value_.dump(0)<<"]";
            }

            virtual bool del( size_t idx ){
                throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
            }

            virtual const IVar& operator []( int idx ) const {
                throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
            }

            virtual IVar& operator []( int idx ){
                throw Lsc::InvalidOperationException()<<BSL_EARG<<"type["<<typeid(*this).name()<<"] this["<<dump(0)<<"] index["<<idx<<"]";
            }

#endif

        private:
            typedef typename implement_t::iterator          iter_impl_t;
            typedef typename implement_t::const_iterator    const_iter_impl_t;
            class DictIteratorImpl;
            class DictConstIteratorImpl;

            class DictIteratorImpl: plclic IDictIteratorImpl{
                friend class DictConstIteratorImpl;
            plclic:

                DictIteratorImpl( const iter_impl_t& iter_ )
                    :_iter(iter_){}

                virtual ~DictIteratorImpl() {
                    //pass
                }

                virtual const string_type& key() const {
                    return _iter->first;
                }

                virtual IVar& value() const {
                    return _iter->second;
                }

                virtual void iterate(){
                    ++ _iter;
                }

                virtual void assign( const IDictIteratorImpl& other ) {
                    const DictIteratorImpl *p = dynamic_cast<const DictIteratorImpl *>(&other);
                    if ( !p ){
                        throw Lsc::BadCastException()<<BSL_EARG<<"from:"<<typeid(other).name()<<" to:DictIteratorImpl";
                    }
                    _iter   = p->_iter;
                }

                virtual bool equal_to( const IDictIteratorImpl& other ) const;

                virtual bool equal_to( const IDictConstIteratorImpl& other ) const;

            private:
                iter_impl_t _iter;
            };


            class DictConstIteratorImpl: plclic IDictConstIteratorImpl{
                friend class DictIteratorImpl;
            plclic:

                DictConstIteratorImpl( const const_iter_impl_t& iter_ )
                    :_iter(iter_){}

                virtual ~DictConstIteratorImpl() {
                    //pass
                }

                virtual const string_type& key() const {
                    return _iter->first;
                }

                virtual const IVar& value() const {
                    return _iter->second;
                }

                virtual void iterate(){
                    ++ _iter;
                }

                virtual void assign( const IDictIteratorImpl& other ) {
                    const DictIteratorImpl *p = dynamic_cast<const DictIteratorImpl *>(&other);
                    if ( !p ){
                        throw Lsc::BadCastException()<<BSL_EARG<<"from:"<<typeid(other).name()<<" to:DictIteratorImpl";
                    }
                    _iter   = p->_iter;
                }

                virtual void assign( const IDictConstIteratorImpl& other ) {
                    const DictConstIteratorImpl *p = dynamic_cast<const DictConstIteratorImpl *>(&other);
                    if ( !p ){
                        throw Lsc::BadCastException()<<BSL_EARG<<"from:"<<typeid(other).name()<<" to:DictIteratorImpl";
                    }
                    _iter   = p->_iter;
                }

                virtual bool equal_to( const IDictConstIteratorImpl& other ) const;

                virtual bool equal_to( const IDictIteratorImpl& other ) const;

            private:
                const_iter_impl_t _iter;
            };

            static IDictIteratorImpl * _s_create_iterator( const iter_impl_t& iter_, const void *p_alloc ){
                typedef typename allocator_type::template rebind<DictIteratorImpl>::other impl_alloc_t;
                IDictIteratorImpl *p = impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).allocate(1);   //throw
                new(p) DictIteratorImpl( iter_ );  //nothrow
                return p;
            }

            static IDictConstIteratorImpl * _s_create_const_iterator( const const_iter_impl_t& iter_, const void *p_alloc ){
                typedef typename allocator_type::template rebind<DictConstIteratorImpl>::other impl_alloc_t;
                IDictConstIteratorImpl *p = impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).allocate(1);   //throw
                new(p) DictConstIteratorImpl( iter_ );  //nothrow
                return p;
            }

            static IDictIteratorImpl * _s_clone_iterator( const IDictIteratorImpl *p_other, const void *p_alloc ){
                typedef typename allocator_type::template rebind<DictIteratorImpl>::other impl_alloc_t;
                const DictIteratorImpl *psrc = dynamic_cast<const DictIteratorImpl*>(p_other);
                if ( !psrc ){
                    throw Lsc::BadCastException()<<BSL_EARG<<"from:"<<typeid(*p_other).name()<<" to:DictIteratorImpl";
                }
                IDictIteratorImpl *p = impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).allocate(1);   //throw
                new(p) DictIteratorImpl(*psrc);  
                return p;
            }

            static IDictConstIteratorImpl * _s_clone_const_iterator( const IDictConstIteratorImpl *p_other, const void *p_alloc ){
                typedef typename allocator_type::template rebind<DictConstIteratorImpl>::other impl_alloc_t;
                const DictConstIteratorImpl *psrc = dynamic_cast<const DictConstIteratorImpl*>(p_other);
                if ( !psrc ){
                    throw Lsc::BadCastException()<<BSL_EARG<<"from:"<<typeid(*p_other).name()<<" to:DictIteratorImpl";
                }
                IDictConstIteratorImpl *p = impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).allocate(1);   //throw
                new(p) DictConstIteratorImpl(*psrc);  
                return p;
            }

            static void _s_destroy_iterator( IDictIteratorImpl * p, const void *p_alloc){
                typedef typename allocator_type::template rebind<DictIteratorImpl>::other impl_alloc_t;
                DictIteratorImpl *_p = dynamic_cast<DictIteratorImpl *>(p); 
                if ( _p ){
                    _p->~DictIteratorImpl();
                    impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).deallocate( _p, 1 );
                }
            }

            static void _s_destroy_const_iterator( IDictConstIteratorImpl * p, const void *p_alloc ){
                typedef typename allocator_type::template rebind<DictConstIteratorImpl>::other impl_alloc_t;
                DictConstIteratorImpl *_p = dynamic_cast<DictConstIteratorImpl *>(p); 
                if ( _p ){
                    _p->~DictConstIteratorImpl();
                    impl_alloc_t(*static_cast<const allocator_type*>(p_alloc)).deallocate( _p, 1 );
                }
            }

            implement_t     _dict;
            allocator_type  _alloc;   //TODO��Ϊ�˼�ֱ�ۣ�û����EBO�Ż����Ժ���Լ���
            Lsc::ResourcePool& _rp;
        };

    template<typename implement_t>
        inline bool MagicBasicDict<implement_t>::DictIteratorImpl::equal_to( const IDictIteratorImpl& other ) const {
            const DictIteratorImpl *p = dynamic_cast<const DictIteratorImpl *>(&other);
            return p != NULL  && _iter == p->_iter;
        }

    template<typename implement_t>
        inline bool MagicBasicDict<implement_t>::DictIteratorImpl::equal_to( const IDictConstIteratorImpl& other ) const {
            const DictConstIteratorImpl *p = dynamic_cast<const DictConstIteratorImpl *>(&other);
            return p != NULL  && p->_iter == _iter;
        }

    template<typename implement_t>
        inline bool MagicBasicDict<implement_t>::DictConstIteratorImpl::equal_to( const IDictIteratorImpl& other ) const {
            const DictIteratorImpl *p = dynamic_cast<const DictIteratorImpl *>(&other);
            return p != NULL  && _iter == p->_iter; 
        }

    template<typename implement_t>
        inline bool MagicBasicDict<implement_t>::DictConstIteratorImpl::equal_to( const IDictConstIteratorImpl& other ) const {
            const DictConstIteratorImpl *p = dynamic_cast<const DictConstIteratorImpl *>(&other);
            return p != NULL  && _iter == p->_iter;
        }

    /**
    *  
    *  ���͵�adapterģʽ
    *  ��adapter����ʹ������ʱ����û����ô����:-P
    *  
    */
    template<template<class>class Alloc>
        class __MagicStdMapAdapter: plclic std::map<IVar::string_type, Lsc::var::MagicRef, std::less<IVar::string_type>, Alloc<std::pair<IVar::string_type, Lsc::var::MagicRef> > >{
        plclic:
            typedef std::map<IVar::string_type, Lsc::var::MagicRef, std::less<IVar::string_type>, Alloc<std::pair<IVar::string_type, Lsc::var::MagicRef> > > base_type;
            typedef typename base_type::allocator_type  allocator_type;
            typedef typename base_type::iterator        iterator;
            typedef typename base_type::const_iterator  const_iterator;

            /**
             *
            **/
            __MagicStdMapAdapter()
                :base_type(){}

            /**
             *
            **/
            explicit __MagicStdMapAdapter( const allocator_type& alloc_ )
                :base_type(std::less<IVar::string_type>(), alloc_ ){}

            /**
             *
             * ����std::map��init_capacityû�����壬ֱ�Ӻ���
            **/
            explicit __MagicStdMapAdapter( size_t /*init_capacity*/, const allocator_type& alloc_ )
                :base_type(std::less<IVar::string_type>(), alloc_ ){}

            //inherit everything else
        };

    /**
    *  
    *  ���͵�adapterģʽ
    *  ��adapter����ʹ������ʱ����û����ô����:-P
    *  
    */
    template<template<class>class Alloc>
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 3
        class __MagicGnuHashMapAdapter: plclic std::unordered_map<
                                        IVar::string_type, 
                                        Lsc::var::MagicRef, 
                                        __gnu_cxx::hash<IVar::string_type>, 
                                        std::equal_to<IVar::string_type>, 
                                        Alloc<Lsc::var::MagicRef> > {
#else
        class __MagicGnuHashMapAdapter: plclic __gnu_cxx::hash_map<
                                        IVar::string_type, 
                                        Lsc::var::MagicRef, 
                                        __gnu_cxx::hash<IVar::string_type>, 
                                        std::equal_to<IVar::string_type>, 
                                        Alloc<Lsc::var::MagicRef> > {
#endif
        plclic:
#if __GNUC__ >= 4 && __GNUC_MINOR__ >= 3
            typedef std::unordered_map<
                IVar::string_type, 
                Lsc::var::MagicRef, 
                __gnu_cxx::hash<IVar::string_type>, 
                std::equal_to<IVar::string_type>, 
                Alloc<Lsc::var::MagicRef> > base_type;
#else
            typedef __gnu_cxx::hash_map<
                IVar::string_type, 
                Lsc::var::MagicRef, 
                __gnu_cxx::hash<IVar::string_type>, 
                std::equal_to<IVar::string_type>, 
                Alloc<Lsc::var::MagicRef> > base_type;
#endif
            typedef typename base_type::allocator_type  allocator_type;
            typedef typename base_type::iterator        iterator;
            typedef typename base_type::const_iterator  const_iterator;

            /**
             *
            **/
            __MagicGnuHashMapAdapter()
                :base_type(){}

            /**
             *
             * 100��__gnu_cxx::hash_mapʹ�õ�Ĭ������ֵ
            **/
            explicit __MagicGnuHashMapAdapter( const allocator_type& alloc_ )
                :base_type(100, typename base_type::hasher(), typename base_type::key_equal(), alloc_ ){}

            /**
             *
            **/
            explicit __MagicGnuHashMapAdapter( size_t init_capacity, const allocator_type& alloc_ )
                :base_type(init_capacity, typename base_type::hasher(), typename base_type::key_equal(), alloc_ ){}

            //inherit everything else
        };
}}   //namespace Lsc::var

#endif  //__BSL_VAR_DICT_H__

/* vim: set ts=4 sw=4 sts=4 tw=100 */
