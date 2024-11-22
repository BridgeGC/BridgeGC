/*
 * Copyright (c) 2018, 2021, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#include "gc/z/zHeap.inline.hpp"
#include "precompiled.hpp"
#include "gc/z/zBarrier.inline.hpp"
#include "gc/z/zBarrierSetRuntime.hpp"
#include "runtime/interfaceSupport.inline.hpp"

JRT_LEAF(oopDesc*, ZBarrierSetRuntime::load_barrier_on_oop_field_preloaded(oopDesc* o, oop* p))
  return ZBarrier::load_barrier_on_oop_field_preloaded(p, o);
JRT_END

JRT_LEAF(void, ZBarrierSetRuntime::check_value(oopDesc* o, oop* p))
//    if(o!=NULL){
        // bool is_in = ZHeap::heap()->is_in(reinterpret_cast<uintptr_t>(o));
        ZCollectedHeap::set_keepObj(p);
//        log_info(gc, heap)("C0: %s, Address: " PTR_FORMAT ", Object: " PTR_FORMAT, o->klass_or_null()->external_name(), reinterpret_cast<uintptr_t>(p), reinterpret_cast<uintptr_t>(o));
//    } else{
//        log_info(gc, heap)("C0: NULLObj : " PTR_FORMAT ", Object: " PTR_FORMAT,  reinterpret_cast<uintptr_t>(p), reinterpret_cast<uintptr_t>(o));
//    }
JRT_END

JRT_LEAF(void, ZBarrierSetRuntime::check_c1_value(oopDesc* o, oop* p))
//    if(o!=NULL){
//        // bool is_in = ZHeap::heap()->is_in(reinterpret_cast<uintptr_t>(o));
//        log_info(gc, heap)("C1: %s, Address: " PTR_FORMAT ", Object: " PTR_FORMAT, o->klass_or_null()->external_name(), reinterpret_cast<uintptr_t>(p), reinterpret_cast<uintptr_t>(o));
        ZCollectedHeap::set_keepObj(p);
//    } else{
//        log_info(gc, heap)("C1: NULLObj : " PTR_FORMAT ", Object: " PTR_FORMAT,  reinterpret_cast<uintptr_t>(p), reinterpret_cast<uintptr_t>(o));
//    }

JRT_END

JRT_LEAF(void, ZBarrierSetRuntime::check_c2_value(oopDesc* o, oop* p))
//    if(o!=NULL){
//        // bool is_in = ZHeap::heap()->is_in(reinterpret_cast<uintptr_t>(o));
//        log_info(gc, heap)("C2: %s, Address: " PTR_FORMAT ", Object: " PTR_FORMAT, o->klass_or_null()->external_name(), reinterpret_cast<uintptr_t>(p), reinterpret_cast<uintptr_t>(o));
        ZCollectedHeap::set_keepObj(p);
//    }else{
//        log_info(gc, heap)("C2: NULLObj : " PTR_FORMAT ", Object: " PTR_FORMAT,  reinterpret_cast<uintptr_t>(p), reinterpret_cast<uintptr_t>(o));
//    }
JRT_END

JRT_LEAF(oopDesc*, ZBarrierSetRuntime::weak_load_barrier_on_oop_field_preloaded(oopDesc* o, oop* p))
  return ZBarrier::weak_load_barrier_on_oop_field_preloaded(p, o);
JRT_END

JRT_LEAF(oopDesc*, ZBarrierSetRuntime::weak_load_barrier_on_weak_oop_field_preloaded(oopDesc* o, oop* p))
  return ZBarrier::weak_load_barrier_on_weak_oop_field_preloaded(p, o);
JRT_END

JRT_LEAF(oopDesc*, ZBarrierSetRuntime::weak_load_barrier_on_phantom_oop_field_preloaded(oopDesc* o, oop* p))
  return ZBarrier::weak_load_barrier_on_phantom_oop_field_preloaded(p, o);
JRT_END

JRT_LEAF(oopDesc*, ZBarrierSetRuntime::load_barrier_on_weak_oop_field_preloaded(oopDesc* o, oop* p))
  return ZBarrier::load_barrier_on_weak_oop_field_preloaded(p, o);
JRT_END

JRT_LEAF(oopDesc*, ZBarrierSetRuntime::load_barrier_on_phantom_oop_field_preloaded(oopDesc* o, oop* p))
  return ZBarrier::load_barrier_on_phantom_oop_field_preloaded(p, o);
JRT_END

JRT_LEAF(void, ZBarrierSetRuntime::load_barrier_on_oop_array(oop* p, size_t length))
  ZBarrier::load_barrier_on_oop_array(p, length);
JRT_END

JRT_LEAF(void, ZBarrierSetRuntime::clone(oopDesc* src, oopDesc* dst, size_t size))
  HeapAccess<>::clone(src, dst, size);
JRT_END

address ZBarrierSetRuntime::load_barrier_on_oop_field_preloaded_addr(DecoratorSet decorators) {
  if (decorators & ON_PHANTOM_OOP_REF) {
    if (decorators & AS_NO_KEEPALIVE) {
      return weak_load_barrier_on_phantom_oop_field_preloaded_addr();
    } else {
      return load_barrier_on_phantom_oop_field_preloaded_addr();
    }
  } else if (decorators & ON_WEAK_OOP_REF) {
    if (decorators & AS_NO_KEEPALIVE) {
      return weak_load_barrier_on_weak_oop_field_preloaded_addr();
    } else {
      return load_barrier_on_weak_oop_field_preloaded_addr();
    }
  } else {
    if (decorators & AS_NO_KEEPALIVE) {
      return weak_load_barrier_on_oop_field_preloaded_addr();
    } else {
      return load_barrier_on_oop_field_preloaded_addr();
    }
  }
}

address ZBarrierSetRuntime::load_barrier_on_oop_field_preloaded_addr() {
  return reinterpret_cast<address>(load_barrier_on_oop_field_preloaded);
}

address ZBarrierSetRuntime::load_barrier_on_weak_oop_field_preloaded_addr() {
  return reinterpret_cast<address>(load_barrier_on_weak_oop_field_preloaded);
}

address ZBarrierSetRuntime::load_barrier_on_phantom_oop_field_preloaded_addr() {
  return reinterpret_cast<address>(load_barrier_on_phantom_oop_field_preloaded);
}

address ZBarrierSetRuntime::weak_load_barrier_on_oop_field_preloaded_addr() {
  return reinterpret_cast<address>(weak_load_barrier_on_oop_field_preloaded);
}

address ZBarrierSetRuntime::weak_load_barrier_on_weak_oop_field_preloaded_addr() {
  return reinterpret_cast<address>(weak_load_barrier_on_weak_oop_field_preloaded);
}

address ZBarrierSetRuntime::weak_load_barrier_on_phantom_oop_field_preloaded_addr() {
  return reinterpret_cast<address>(weak_load_barrier_on_phantom_oop_field_preloaded);
}

address ZBarrierSetRuntime::load_barrier_on_oop_array_addr() {
  return reinterpret_cast<address>(load_barrier_on_oop_array);
}

address ZBarrierSetRuntime::clone_addr() {
  return reinterpret_cast<address>(clone);
}

address ZBarrierSetRuntime::check_address_value(){
    return reinterpret_cast<address>(check_value);
}

address ZBarrierSetRuntime::check_c1_address_value(){
    return reinterpret_cast<address>(check_c1_value);
}

address ZBarrierSetRuntime::check_c2_address_value(){
    return reinterpret_cast<address>(check_c2_value);
}
