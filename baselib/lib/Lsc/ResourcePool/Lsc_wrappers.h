
#ifndef  __BSL_WRAPPERS_H_
#define  __BSL_WRAPPERS_H_

#include <memory>
#include <iostream>
#include <cstdio>
#include "Lsc/ResourcePool/Lsc_wrappers_config.h"

/**
 *  
 *  ������������ģ��malloc()��placement operator new����Ϊ,����߷���POD������ٶ�
 *  ��������������ģ��calloc(), ��ͨoperator new��STL��������Ϊ��
 */
//#define BSL_CONFIG_SKIP_POD_INITIALIAZION

/**
 *  
 *  ������������ģ��operator new����Ϊ
 *  ��������������ģ��STL��������Ϊ
 */
//#define BSL_CONFIG_DO_NOT_COPY_FOR_DEFAULT_CONSTRUCTION

namespace Lsc{
    /**
     *
     * ע���ڲ�ʹ�ã��û���Ӧֱ�ӵ��øú�����
     *
     **/
    template<typename T>
        inline void __construct_aux(void * p_object, __BSL_TRUE_TYPE__){
#ifndef BSL_CONFIG_SKIP_POD_INITIALIAZION
            *static_cast<T*>(p_object) = 0;
#endif
        }

    /**
     *
     * ע���ڲ�ʹ�ã��û���Ӧֱ�ӵ��øú�����
     *
     **/
    template<typename T>
        inline void __construct_aux(void * p_object, __BSL_FALSE_TYPE__){
            ::new(p_object) T();    
        }

    /**
     *  
     *  
     */
    template<typename T>
        inline void construct(void * p_object){
            typedef __BSL_HAS_TRIVIAL_DEFAULT_CONSTRUCTOR(T) __is_POD;
            __construct_aux<T>(p_object, __is_POD() );
        }
    /**
     *  
     *  
     */
    template<typename T, typename T_arg>
        inline void construct(void * p_object, const void * p_arg ){
            new(p_object) T(*static_cast<const T_arg*>(p_arg));
        }

    /**
     *
     **/
    template<typename T>
        inline void destroy(void * p_object){
            static_cast<T*>(p_object)->~T();
        }

    /**
     *
     * ע���ڲ�ʹ�ã��û���Ӧֱ�ӵ��øú�����
     **/
    template<typename T>
        inline void __construct_array_aux( void * begin, void * end, __BSL_FALSE_TYPE__){
#ifndef BSL_CONFIG_DO_NOT_COPY_FOR_DEFAULT_CONSTRUCTION
            std::uninitialized_fill( static_cast<T*>(begin), static_cast<T*>(end), T() ); 
#else
            ::new(begin) T[static_cast<T*>(end) - static_cast<T*>(begin)];
#endif 
        }

    /**
     *
     * ע���ڲ�ʹ�ã��û���Ӧֱ�ӵ��øú�����
     **/
    template<typename T>
        inline void __construct_array_aux( void * begin, void * end, __BSL_TRUE_TYPE__){
#ifndef BSL_CONFIG_SKIP_POD_INITIALIAZION
            memset(begin, 0, static_cast<char*>(end) - static_cast<char*>(begin) );
#endif
        }

    /**
     *
     **/
    template<typename T>
        inline void construct_array(void * begin, void * end ){
            typedef __BSL_HAS_TRIVIAL_DEFAULT_CONSTRUCTOR(T) __is_POD;
            __construct_array_aux<T>( begin, end, __is_POD() );
        }

    /**
     *
     **/
    template<typename T, typename T_arg >
        inline void construct_array(void * begin, void * end, const void * src ){
            std::uninitialized_fill( static_cast<T *>(begin), static_cast<T *>(end), *static_cast<const T_arg *>(src) ); // this method will be specially optimized to POD;
        }

    /**
     *
     **/
    template<typename T>
        inline void default_destroy_array(T* begin, T* end){
            while( begin != end ){
                (begin ++) -> ~T();
            }
        }

    /**
     *
     **/
    template<typename T>
        inline void destroy_array( void * begin, void * end ){
            __BSL_DESTROY(static_cast<T*>(begin), static_cast<T*>(end)); // this method will be specially optimized to the type which has trivial destructor;
        }

    /**
     * ���ö��ڴ治����Alloc������ͷŻ� end - begin ) �� sizeof(Alloc::value_type) != 0������δ����
     **/
    template<typename Alloc >
        inline void deallocate( void * begin, void * end ){
            typedef typename Alloc::value_type value_t;
            value_t * vt_begin = static_cast<value_t *>(begin);
            value_t * vt_end   = static_cast<value_t *>(end);
            Alloc().deallocate( vt_begin, vt_end - vt_begin );
        }

    /**
     *
     **/
    template<typename T, typename Alloc>
        inline void destroy_and_deallocate( void * p_object ){
            static_cast<T*>(p_object)->~T();
            typedef typename Alloc::template rebind<T>::other T_Alloc;
            T_Alloc().deallocate(static_cast<T *>(p_object), 1);
        }

    /**
     *
     **/
    template<typename T, typename Alloc>
        inline void destroy_and_deallocate_array( void * begin, void * end ){
            __BSL_DESTROY(static_cast<T*>(begin), static_cast<T*>(end)); // this method will be specially optimized to the type which has trivial destructor;
            typedef typename Alloc::template rebind<T>::other T_Alloc;
            T_Alloc().deallocate(static_cast<T *>(begin), static_cast<T*>(end) - static_cast<T*>(begin) );
        }

    /**
     *
     **/
    template<typename T>
        inline void Lsc_delete(void * object ){
            delete static_cast<T*>(object);
        }

    /**
     *
     **/
    template<typename T>
        inline void Lsc_delete_array( void * array ){
            delete[] static_cast<T*>(array);
        }

    /**
     *
     * ��BSL�У�����ʹ�øú���������ϵͳ����free(void*)
     *
     **/
    inline void Lsc_free( void *data){
#ifdef BSL_DEBUG_FLAG
        // ���������Ϣ
        std::cerr<<"memory "<<data<<" freeed"<<std::endl;
#endif
        free(data);
    }

    /**
     *
     * ��BSL�У�����ʹ�øú���������ϵͳ����fclose(FILE*)
     **/
    inline void Lsc_fclose( void *data){
#ifdef BSL_DEBUG_FLAG
        // ���������Ϣ
        std::cerr<<"file "<<data<<" closed"<<std::endl;
#endif
        fclose(static_cast<FILE*>(data));
    }

}   //namespace Lsc
#endif  //__BSL_WRAPPERS_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
