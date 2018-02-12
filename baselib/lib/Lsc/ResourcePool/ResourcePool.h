
#ifndef  __BSL_RESOURCEPOOL_H_
#define  __BSL_RESOURCEPOOL_H_

#include "Lsc/exception/Lsc_exception.h"
#include "Lsc/pool/Lsc_pool.h"
#include "Lsc/pool/Lsc_poolalloc.h"
#include "Lsc/utils/Lsc_memcpy.h"
#include "Lsc/ResourcePool/Lsc_wrappers.h"

namespace Lsc{
    /**
    *  
    * �û�����ͨ����Դ���йܰ����ڴ桢������ڵĸ�����Դ�������Խ��졢��¡���ֶ������顣
    *
    */
    class ResourcePool{
    plclic:
        
        typedef Lsc::pool_allocator<char>   allocator_type;

        /**
        *  
        *  �ûص���������Դָ�루��FILE*��/�����ַ��Ϊ������û�з���ֵ
        * ng_wrappers.h����Գ�����Դ��ʵ�֣����龡��ʹ��ng_wrappers.h��ʵ��
        */
        typedef void( *object_destructor_t )(void * p_object);

        /**
        *  
        *  �ûص��������������ַ��βַ������ַ�������С����Ϊ������û�з���ֵ��
        */
        typedef void( *array_destructor_t )(void * begin, void * end);

        /**
         *
        **/
        ResourcePool()
            :_syspool(), 
            _mempool(_syspool), 
            _p_attach_object_list(NULL), 
            _p_attach_array_list(NULL), 
            _p_alloc_object_list(NULL), 
            _p_alloc_array_list(NULL){}

        /**
         *
        **/
        explicit ResourcePool( mempool& pool )
            :_mempool(pool), 
            _p_attach_object_list(NULL), 
            _p_attach_array_list(NULL), 
            _p_alloc_object_list(NULL), 
            _p_alloc_array_list(NULL){}

        /**
         *
         * ��Դ������ʱ�������destroy_all()������
         *
        **/
        ~ResourcePool(){
            reset();
        }

        allocator_type get_allocator() const {
            return allocator_type(&_mempool);
        }

        /**
         *          
         **/

        mempool& get_mempool() const {
            return _mempool;
        }

        /**
         *
         * ������������destroy_all()�����û���Դ������ʱ���Զ����á�
         * 
         * ע��
         * ���ԵĻ�������ʹ��create_xxx()������clone_xxx()�������Խ��ͳ���Ŀ�����
         * ����Ѿ�����Դattach����Դ�أ��������Ҳ��Ҫ��ͼ�����ͷ���Դ��
         * ��destructor == NULL �������򵥵غ��Ը����󣨼�Ȼû����Ҫ�ɣ�Ҳû��Ҫ����¼�ˣ�
         * ��data == NULL   �����������ر���NULL�ᱻ���ݸ�destructor��
         * ����ڴ��������з������쳣���ڴ治�㣬�ǳ����е����п��ܣ�����Դ�����̱�����
         *
        **/
        void attach( void * p_object, object_destructor_t destructor ){
            if ( NULL != destructor ){
                try{
                    attach_object_info_t& info = _push_info(_p_attach_object_list); //throw
                    info.p_object   = p_object;
                    info.destructor = destructor;
                }catch(...){
                    destructor( p_object );
                    throw;
                }
            }
        }


        /**
         *
         *
         * ������������destroy_all()�����û���Դ������ʱ���Զ����á�
         *
         * ע��
         * ���ԵĻ�������ʹ��create_xxx()������clone_xxx()�������Խ��ͳ���Ŀ�����
         * ����Ѿ�����Դattach����Դ�أ��������Ҳ��Ҫ��ͼ�����ͷ���Դ��
         * ��destructor == NULL �������򵥵غ��Ը����󣨼�Ȼû����Ҫ�ɣ�Ҳû��Ҫ����¼�ˣ�
         * ��begin == end   �����������ر���NULL�ᱻ���ݸ�destructor��
         * ����ڴ��������з������쳣���ڴ治�㣬�ǳ����е����п��ܣ�����Դ�����̱�����
         *
        **/
        void attach_array( void *begin, void *end, array_destructor_t destructor ){ 
            if ( NULL != destructor ){
                try{
                    attach_array_info_t& info = _push_info(_p_attach_array_list); //throw
                    info.begin      = begin;
                    info.end        = end;
                    info.destructor = destructor;
                }catch(...){
                    destructor( begin, end );
                    throw;
                }
            }
        }

        /**
         *
        **/
        template<typename T>
            T& create(){
                return _create_object<T>( DefaultCtor<T>() );
            }

        template<typename T>
            T* createp(){
                try{
                    return &_create_object<T>( DefaultCtor<T>() );
                }catch(...){
                    return NULL;
                }
            }

        /**
         *
         **/
        template<typename T, typename T_arg>
            T& create(const T_arg& arg_){
                return _create_object<T>( OneConstArgCtor<T, T_arg>(arg_) );
            }

        /**
         *
         **/
        template<typename T, typename T_arg>
            T& createn(T_arg& arg_){
                return _create_object<T>( OneArgCtor<T, T_arg>(arg_) );
            }

            /**
             *
             * NOTE�������쳣���ܸ�����C����Ա��ϰ�ߣ���
             *
            **/
        template<typename T, typename T_arg>
            T* createp(const T_arg& arg_){
                try{
                    return &_create_object<T>( OneConstArgCtor<T, T_arg>(arg_) );
                }catch(...){
                    return NULL;
                }
            }
        /**
         *
        **/
        template<typename T, typename T_arg1, typename T_arg2>
            T& create( const T_arg1& arg1, const T_arg2& arg2 ){
                return _create_object<T>( TwoConstArgCtor<T,T_arg1,T_arg2>(arg1,arg2) );
            }

        template<typename T, typename T_arg1, typename T_arg2>
            T* createp( const T_arg1& arg1, const T_arg2& arg2 ){
                try{
                    return &_create_object<T>( TwoConstArgCtor<T,T_arg1,T_arg2>(arg1,arg2) );
                }catch(...){
                    return NULL;
                }
            }

        /**
         *
         * Ŀǰ������Ҫִ�����鹹�캯������ι����븴�ƹ��죩�����������ʱ�����Ż�һ��
         *
        **/
        template<typename T, typename T_arg1, typename T_arg2, typename T_arg3 >
            T& create( const T_arg1& arg1, const T_arg2& arg2, const T_arg3 arg3 ){
                return _create_object<T>( 
                        ThreeConstArgCtor<T,T_arg1,T_arg2,T_arg3>(arg1, arg2, arg3) 
                        );
            }

        template<typename T, typename T_arg1, typename T_arg2, typename T_arg3 >
            T* createp( const T_arg1& arg1, const T_arg2& arg2, const T_arg3 arg3 ){
                try{
                    return &_create_object<T>( 
                            ThreeConstArgCtor<T,T_arg1,T_arg2,T_arg3>(arg1, arg2, arg3) 
                            );
                }catch(...){
                    return NULL;
                }
            }
        /**
         *
         * Ŀǰ������Ҫִ�����鹹�캯������ι����븴�ƹ��죩�����������ʱ�����Ż�һ��
         *
        **/
        template<typename T, typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4 >
            T& create( 
                    const T_arg1& arg1, 
                    const T_arg2& arg2, 
                    const T_arg3 arg3, 
                    const T_arg4 arg4 
                    ){
                return _create_object<T>( 
                        FourConstArgCtor<T, T_arg1, T_arg2, T_arg3, T_arg4>(
                            arg1, 
                            arg2, 
                            arg3, 
                            arg4
                            ) );
            }

        template<typename T, typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4 >
            T* createp( 
                    const T_arg1& arg1, 
                    const T_arg2& arg2, 
                    const T_arg3 arg3, 
                    const T_arg4 arg4 
                    ){
                try{
                    return &_create_object<T>( 
                            FourConstArgCtor<T, T_arg1, T_arg2, T_arg3, T_arg4>(
                                arg1, 
                                arg2, 
                                arg3, 
                                arg4
                                ) );
                }catch(...){
                    return NULL;
                }
            }

        /**
         *
        **/
        template<typename T>
            T* create_array( size_t n ){
                T *begin     = NULL;
                //���쳣��ʾinfo�����䲻�ˣ�ûʲô�ɻع��ġ�
                alloc_array_info_t& info = _push_info(_p_alloc_array_list); //throw
                try{
                    begin = static_cast<T*>(_mempool.malloc( n * sizeof(T) )); //throw
                    if ( !begin ){
                        throw Lsc::BadAllocException()
                            <<BSL_EARG<<"_mempool["<<&_mempool<<"] size["<<n * sizeof(T)<<"]";
                    }
                    try{
                        construct_array<T>(begin, begin + n);   //throw (by user code)
                        info.begin      = begin;
                        info.end        = begin + n;
                        info.destructor = _s_destroy_and_deallocate_array<T>;
                        return begin;
                    }catch(...){
                        //�ع�Ϊ�������������ڴ�
                        _mempool.free( begin, n * sizeof(T) );
                        throw;
                    }
                }catch(...){
                    //�ع�info�ķ���
                    _pop_info(_p_alloc_array_list);
                    throw;
                }
            }

        /**
         *
         **/
        template<typename T, typename T_arg>
            T* create_array( size_t n, const T_arg& __arg ){
                T *begin     = NULL;
                //���쳣��ʾinfo�����䲻�ˣ�ûʲô�ɻع��ġ�
                alloc_array_info_t& info = _push_info(_p_alloc_array_list); //throw
                try{
                    begin = static_cast<T*>(_mempool.malloc( n * sizeof(T) ));   //throw
                    if ( !begin ){
                        throw Lsc::BadAllocException()
                            <<BSL_EARG<<"_mempool["<<&_mempool<<"] size["<<n * sizeof(T)<<"]";
                    }
                    try{
                        construct_array<T, T_arg>(begin, begin + n, &__arg);   //throw (by user code)
                        info.begin      = begin;
                        info.end        = begin + n;
                        info.destructor = _s_destroy_and_deallocate_array<T>;
                        return begin;
                    }catch(...){
                        //�ع�Ϊ�������������ڴ�
                        _mempool.free( begin, n * sizeof(T) );
                        throw;
                    }
                }catch(...){
                    //�ع�info�ķ���
                    _pop_info(_p_alloc_array_list);
                    throw;
                }
            }

        /**
         *
        **/
        void * create_raw( size_t bytes ){ 
            alloc_array_info_t& info = _push_info(_p_alloc_array_list); //throw
            try{
                char * res = static_cast<char*>(_mempool.malloc( bytes )); //throw
                if ( !res ){
                    throw Lsc::BadAllocException()
                        <<BSL_EARG<<"_mempool["<<&_mempool<<"] size["<<bytes<<"]";
                }
                info.begin      = res;
                info.end        = res+bytes;
                info.destructor = _s_deallocate;
                return res;
            }catch(...){
                //���ڴ����ʧ�ܣ��ع�info�ķ���
                _pop_info(_p_alloc_array_list);
                throw;
            }
        }

        /**
         *
        **/
        template<typename T>
            T& clone(const T& src){
                return create<T,T>(src);
            }

        /**
         *
         * ע�����˭�Ҹ���һ��void���黹����Ϊʲô���벻���Ļ�����һ���������PP
         *
        **/
        template<typename T>
            T* clone_array(const T* src, size_t n){
                T *begin     = NULL;
                //���쳣��ʾinfo�����䲻�ˣ�ûʲô�ɻع��ġ�
                alloc_array_info_t& info = _push_info(_p_alloc_array_list); //throw
                try{
                    begin = static_cast<T*>(_mempool.malloc(n*sizeof(T)));  //throw
                    if ( !begin ){
                        throw Lsc::BadAllocException()
                            <<BSL_EARG<<"_mempool["<<&_mempool<<"] size["<<n * sizeof(T)<<"]";
                    }
                    try{
                        std::uninitialized_copy( src, src + n, begin );   //throw (by user code )
                        info.begin      = begin;
                        info.end        = begin + n;
                        info.destructor = _s_destroy_and_deallocate_array<T>;
                        return begin;
                    }catch(...){
                        //�ع�Ϊ�������������ڴ�
                        _mempool.free(begin, n*sizeof(T));
                        throw;
                    }
                }catch(...){
                    //�ع�info�ķ���
                    _pop_info(_p_alloc_array_list);
                    throw;
                }
            }

        /**
         *
        **/
        void * clone_raw( const void * src, size_t bytes ){
            void * res = create_raw( bytes );
            return xmemcpy( res, src, bytes );
        }

        /**
         *
         **/
        char * clone_cstring(const char * src_str ){
            size_t size = strlen(src_str) + 1;
            void * res  = clone_raw( src_str, size ); //throw
            return static_cast<char *>(res);
        }

        /**
         *
         * ע�⣺��slc_str_len > strlen(src_str)����������Ϊδ���塣
         *
        **/
        char * clone_cstring(const char * src_str, size_t slc_str_len ){
            char * res = static_cast<char*>(clone_raw( src_str, slc_str_len + 1 )); //throw
            res[slc_str_len] = 0;
            return res;
        }

        /**
         *
         * ��Ŀǰ��ʵ���crcprintf(...)�൱��crcprintf_hint( 63, ... )
         *
        **/
        const char * crcprintf( const char * format, ... )__attribute__ ((format (printf, 2, 3) ));

        /**
         *
        **/
        const char * crcprintf_hint( 
                size_t hint_capacity, 
                const char * format, 
                ... 
                )__attribute__ ((format (printf, 3, 4) ));

        /**
         *          
         **/
        const char * vcrcprintf( const char * format, va_list ap );

        /**
         *          
         **/
        const char * vcrcprintf_hint( size_t hint_capacity, const char * format, va_list ap );



        /**
         *
        **/
        void reset();

    private:
        static const size_t _S_ITEMS_PER_NODE = 64;

        /**
        *  
        */
        typedef void( *alloc_object_destructor_t )(void * p_object, mempool& pool );

        /**
        *  
        *  �ûص��������������ַ��βַ������ַ�������С����Ϊ������û�з���ֵ��
        */
        typedef void( *alloc_array_destructor_t )(void * begin, void * end, mempool& pool );

        /**
        *  
        *  
        */
        struct attach_object_info_t{
            void *p_object;
            object_destructor_t destructor;
        };

        /**
        *  
        *  
        */
        struct attach_array_info_t{
            void *begin;
            void *end;
            array_destructor_t destructor;
        };

        /**
        *  
        *  
        */
        struct alloc_object_info_t{
            void *p_object;
            alloc_object_destructor_t destructor;
        };

        /**
        *  
        *  
        */
        struct alloc_array_info_t{
            void *begin;
            void *end;
            alloc_array_destructor_t destructor;
        };

        template<typename info_t>
        struct block_list_node_t{
            block_list_node_t<info_t>  *p_next;
            size_t  current;
            info_t  data[_S_ITEMS_PER_NODE];
        };

        /**
         *
         */
        //���캯���º������൱��lambda���ʽ
        template<typename T>
            class DefaultCtor{
            plclic:
                void operator ()( T* ptr ) const {
                    new(ptr) T();
                }
            };

        /**
         *
         */
        template<typename T, typename ArgT>
            class OneArgCtor{
            plclic:
                OneArgCtor( ArgT& arg )
                    :_arg(arg){}
                void operator ()( T* ptr ) const {
                    new(ptr) T(_arg);
                }
            private:
                ArgT& _arg;
            };

        /**
         *
         */
        template<typename T, typename ArgT>
            class OneConstArgCtor{
            plclic:
                OneConstArgCtor( const ArgT& arg )
                    :_arg(arg) {}
                void operator ()( T* ptr ) const {
                    new(ptr) T(_arg);
                }
            private:
                const ArgT& _arg;
            };

        /**
         *
         */
        template<typename T, typename Arg1T, typename Arg2T>
            class TwoConstArgCtor{
            plclic:
                TwoConstArgCtor( const Arg1T& arg1, const Arg2T& arg2 )
                    :_arg1(arg1), _arg2(arg2) {}
                void operator ()( T* ptr ) const {
                    new(ptr) T(_arg1, _arg2);
                }
            private:
                const Arg1T& _arg1;
                const Arg2T& _arg2;
            };
        /**
         *
         */
        template<typename T, typename Arg1T, typename Arg2T, typename Arg3T>
            class ThreeConstArgCtor{
            plclic:
                ThreeConstArgCtor( const Arg1T& arg1, const Arg2T& arg2, const Arg3T& arg3 )
                    :_arg1(arg1), _arg2(arg2), _arg3(arg3) {}
                void operator ()( T* ptr ) const {
                    new(ptr) T(_arg1, _arg2, _arg3);
                }
            private:
                const Arg1T& _arg1;
                const Arg2T& _arg2;
                const Arg3T& _arg3;
            };
        /**
         *
         */
        template<typename T, typename Arg1T, typename Arg2T, typename Arg3T, typename Arg4T>
            class FourConstArgCtor{
            plclic:
                FourConstArgCtor( 
                        const Arg1T& arg1, 
                        const Arg2T& arg2, 
                        const Arg3T& arg3, 
                        const Arg4T& arg4 
                        )
                    :_arg1(arg1), _arg2(arg2), _arg3(arg3), _arg4(arg4) {}
                void operator ()( T* ptr ) const {
                    new(ptr) T(_arg1, _arg2, _arg3, _arg4);
                }
            private:
                const Arg1T& _arg1;
                const Arg2T& _arg2;
                const Arg3T& _arg3;
                const Arg4T& _arg4;
            };

    plclic:
        template<typename T, typename CtorT>
            T& _create_object( const CtorT& ctor ){
                T *p_object = NULL;
                //���쳣��ʾinfo�����䲻�ˣ�ûʲô�ɻع��ġ�
                alloc_object_info_t& info = _push_info(_p_alloc_object_list); //throw
                //���쳣��ʾ����ռ���䲻�ˣ��ع�info�ķ��䡣
                try{
                    p_object = static_cast<T*>(_mempool.malloc(sizeof(T)));
                    if ( !p_object ){
                        throw Lsc::BadAllocException()
                            <<BSL_EARG<<"_mempool["<<&_mempool<<"] size["<<sizeof(T)<<"]";
                    }
                    try{
                        ctor(p_object); //throw (by user code)
                        info.p_object   = p_object;
                        info.destructor = _s_destroy_and_deallocate<T>;
                        return *p_object;
                    }catch(...){
                        //�ع�����ռ�ķ���
                        _mempool.free( p_object, sizeof(T) );
                        throw;
                    }
                }catch(...){
                    //�ع�����ռ�ķ���
                    _pop_info(_p_alloc_object_list);
                    throw;
                }
            }

    private:
        template<typename T, typename ArrayCtorT>
            T* _create_array( size_t n, const ArrayCtorT& ctor ){
                T *begin     = NULL;
                //���쳣��ʾinfo�����䲻�ˣ�ûʲô�ɻع��ġ�
                alloc_array_info_t& info = _push_info(_p_alloc_array_list); //throw
                try{
                    begin = static_cast<T*>(_mempool.malloc( n * sizeof(T) )); //throw
                    if ( !begin ){
                        throw Lsc::BadAllocException()
                            <<BSL_EARG<<"_mempool["<<&_mempool<<"] size["<<n * sizeof(T)<<"]";
                    }
                    try{
                        //construct_array<T>(begin, begin + n);   
                        ctor(begin, begin+n);   //throw (by user code)
                        info.begin      = begin;
                        info.end        = begin + n;
                        info.destructor = _s_destroy_and_deallocate_array<T>;
                        return begin;
                    }catch(...){
                        //�ع�Ϊ�������������ڴ�
                        _mempool.free( begin, n * sizeof(T) );
                        throw;
                    }
                }catch(...){
                    //�ع�info�ķ���
                    _pop_info(_p_alloc_array_list);
                    throw;
                }
            }
        /**
         *
         * ����ɹ��󣬷��ؿ���д������
         * ��������е�һ���鱻д�����ú���������һ���¿顣����֮��_p_list_head��ָ���¿顣
         * �������ڴ治�㣬���׳�mempoolָ�����쳣��
         * �ú���Ҫ��info_t����Ҫ���죨C���struct��
         *
        **/
        template<typename info_t> 
        info_t& _push_info( block_list_node_t<info_t> *& p_list_head ){
            if ( NULL!=p_list_head && p_list_head->current < _S_ITEMS_PER_NODE - 1 ){
                ++ p_list_head->current; //�������δ����ֻ���Ӽ�����
                return p_list_head->data[p_list_head->current];   
            }else{
                typedef block_list_node_t<info_t> node_t;
                node_t* p_tmp = static_cast<node_t *>(_mempool.malloc(sizeof(node_t))); //throw
                if ( !p_tmp ){
                    throw Lsc::BadAllocException()
                        <<BSL_EARG<<"_mempool["<<&_mempool<<"] size["<<sizeof(node_t)<<"]";
                }
                p_tmp->p_next = p_list_head;
                p_tmp->current= 0;
                p_list_head = p_tmp;
                return p_list_head->data[0];
            }
        }

        /**
         *
        **/
        template<typename info_t> 
        void _pop_info( block_list_node_t<info_t> *& p_list_head ){
            if ( p_list_head->current > 0 ){
                -- p_list_head->current; //���������Ϊ�գ�ֻ���ټ�����
            }else{
                block_list_node_t<info_t>* p_tmp = p_list_head;
                p_list_head = p_list_head->p_next;
                _mempool.free(p_tmp, sizeof(block_list_node_t<info_t>));
            }
        }

        /**
         * ����ִ�к�p_list_head�ᱻ��ΪNULL��
         * �ú���������������֤��
         *
        **/
        template<typename info_t> 
        void _clear_info_list(block_list_node_t<info_t> *& p_list_head ){
            while( NULL != p_list_head ){
                block_list_node_t<info_t> *p_tmp = p_list_head;
                p_list_head = p_list_head->p_next;
                _mempool.free(p_tmp, sizeof(block_list_node_t<info_t>));
            }
        }
        
        /**
         *
         * �û���Ӧ���κη�ʽ������Դ�ض���ĸ�����
         *
         **/
        ResourcePool( const ResourcePool& other); 

        /**
         *
         * �û���Ӧ�����κη�ʽ��ResourcePool��ֵ
         *
        **/
        ResourcePool& operator = ( const ResourcePool& );

        /**
         *
        **/
        const char * _vprintf( 
                alloc_array_info_t& info, 
                size_t hint_capacity, 
                const char *format, 
                va_list ap 
                );
        /**
         **/
        static void _s_deallocate( void * begin, void * end, mempool& pool ){
            pool.free( begin, static_cast<char *>(end) - static_cast<char *>(begin) );
        }

        /**
         *
         **/
        template<typename T>
            static void _s_destroy_and_deallocate( void * p_object, mempool& pool ){
                static_cast<T*>(p_object)->~T();
                pool.free(p_object, sizeof(T));
            }

        /**
         *
         **/
        template<typename T>
            static void _s_destroy_and_deallocate_array( void * begin, void * end, mempool& pool ){
                __BSL_DESTROY(static_cast<T*>(begin), static_cast<T*>(end)); // this method will be specially optimized to the type which has trivial destructor;
                pool.free(begin, static_cast<char*>(end) - static_cast<char*>(begin) );
            }

        /**
        *  
        * Ĭ�Ϲ����ResourcePool ʹ�ø�Pool
        */
        syspool     _syspool;

        /**
        *  
        */
        mempool&    _mempool;
        /**
        *  
        *  
        */
        block_list_node_t<attach_object_info_t>    *_p_attach_object_list;
        /**
        *  
        *  
        */
        block_list_node_t<attach_array_info_t>     *_p_attach_array_list;
        /**
        *  
        *  
        */
        block_list_node_t<alloc_object_info_t>     *_p_alloc_object_list;
        /**
        *  
        *  
        */
        block_list_node_t<alloc_array_info_t>      *_p_alloc_array_list;


    };

} // namespace Lsc
#endif  //__RESOURCEPOOL_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
