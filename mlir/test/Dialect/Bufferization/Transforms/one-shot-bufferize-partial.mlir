// RUN: mlir-opt %s -allow-unregistered-dialect -one-shot-bufferize="allow-unknown-ops" -split-input-file | FileCheck %s

// Test bufferization using memref types that have no layout map.
// RUN: mlir-opt %s -allow-unregistered-dialect -one-shot-bufferize="allow-unknown-ops unknown-type-conversion=identity-layout-map" -split-input-file | FileCheck %s --check-prefix=CHECK-NO-LAYOUT-MAP

// Run fuzzer with different seeds.
// RUN: mlir-opt %s -allow-unregistered-dialect -one-shot-bufferize="test-analysis-only analysis-heuristic=fuzzer analysis-fuzzer-seed=23" -split-input-file -o /dev/null
// RUN: mlir-opt %s -allow-unregistered-dialect -one-shot-bufferize="test-analysis-only analysis-heuristic=fuzzer analysis-fuzzer-seed=59" -split-input-file -o /dev/null
// RUN: mlir-opt %s -allow-unregistered-dialect -one-shot-bufferize="test-analysis-only analysis-heuristic=fuzzer analysis-fuzzer-seed=91" -split-input-file -o /dev/null

// RUN: mlir-opt %s -allow-unregistered-dialect -one-shot-bufferize="dialect-filter=tensor,bufferization allow-unknown-ops" -canonicalize -split-input-file | FileCheck %s --check-prefix=CHECK-TENSOR
// RUN: mlir-opt %s -allow-unregistered-dialect -one-shot-bufferize="dialect-filter=scf,bufferization allow-unknown-ops" -canonicalize -split-input-file | FileCheck %s --check-prefix=CHECK-SCF

// CHECK-LABEL: func @use_of_unknown_op_1(
//  CHECK-SAME:     %[[t1:.*]]: tensor<?xf32>
// CHECK-NO-LAYOUT-MAP-LABEL: func @use_of_unknown_op_1(
//  CHECK-NO-LAYOUT-MAP-SAME:     %[[t1:.*]]: tensor<?xf32>
func.func @use_of_unknown_op_1(%t1: tensor<?xf32>)
    -> vector<5xf32> {
  // ToTensorOp is generated because the function is bufferized and has a
  // memref block argument.
  // CHECK: %[[dummy:.*]] = "test.dummy_op"(%[[t1]])
  // CHECK-NO-LAYOUT-MAP: %[[dummy:.*]] = "test.dummy_op"(%[[t1]])
  %0 = "test.dummy_op"(%t1) : (tensor<?xf32>) -> tensor<?xf32>

  %idx = arith.constant 0 : index
  %cst = arith.constant 0.0 : f32
  // CHECK: %[[dummy_memref:.*]] = bufferization.to_memref %[[dummy]] : tensor<?xf32> to memref<?xf32, strided<[?], offset: ?>>
  // CHECK: vector.transfer_read %[[dummy_memref]][%{{.*}}], %{{.*}} : memref<?xf32, strided<[?], offset: ?>>
  // CHECK-NO-LAYOUT-MAP: %[[dummy_memref:.*]] = bufferization.to_memref %[[dummy]] : tensor<?xf32> to memref<?xf32>
  // CHECK-NO-LAYOUT-MAP: vector.transfer_read %[[dummy_memref]][%{{.*}}], %{{.*}} : memref<?xf32>
  %1 = vector.transfer_read %0[%idx], %cst : tensor<?xf32>, vector<5xf32>
  return %1 : vector<5xf32>
}

// -----

// CHECK-LABEL: func @use_of_unknown_op_2(
//  CHECK-SAME:     %[[t1:.*]]: tensor<?xf32>
func.func @use_of_unknown_op_2(%t1: tensor<?xf32>) -> tensor<?xf32> {
  // CHECK: %[[dummy1:.*]] = "test.dummy_op"(%[[t1]])
  %0 = "test.dummy_op"(%t1) : (tensor<?xf32>) -> tensor<?xf32>
  // CHECK: %[[dummy2:.*]] = "test.another_dummy_op"(%[[dummy1]])
  %1 = "test.another_dummy_op"(%0) : (tensor<?xf32>) -> tensor<?xf32>

  // CHECK: return %[[dummy2]]
  return %1 : tensor<?xf32>
}

// -----

// CHECK-LABEL: func @use_of_unknown_op_3(
//  CHECK-SAME:     %[[t1:.*]]: tensor<?xf32>
func.func @use_of_unknown_op_3(%t1: tensor<?xf32>)
    -> (vector<5xf32>, vector<5xf32>) {
  %idx = arith.constant 0 : index
  %cst = arith.constant 0.0 : f32
  // CHECK: %[[m1:.*]] = bufferization.to_memref %[[t1]]
  // CHECK: %[[v1:.*]] = vector.transfer_read %[[m1]]
  %1 = vector.transfer_read %t1[%idx], %cst : tensor<?xf32>, vector<5xf32>

  // CHECK: %[[dummy:.*]] = "test.dummy_op"(%[[t1]])
  %0 = "test.dummy_op"(%t1) : (tensor<?xf32>) -> tensor<?xf32>
  // CHECK: %[[dummy_memref:.*]] = bufferization.to_memref %[[dummy]] : tensor<?xf32> to memref<?xf32, strided<[?], offset: ?>>
  // CHECK: %[[v2:.*]] = vector.transfer_read %[[dummy_memref]]
  %2 = vector.transfer_read %0[%idx], %cst : tensor<?xf32>, vector<5xf32>

  // CHECK: return %[[v1]], %[[v2]]
  return %1, %2 : vector<5xf32>, vector<5xf32>
}

// -----

// CHECK-LABEL: func @use_of_unknown_op_4(
//  CHECK-SAME:     %[[t1:.*]]: tensor<?xf32>
func.func @use_of_unknown_op_4(%t1: tensor<?xf32>)
    -> (vector<5xf32>, tensor<?xf32>) {
  %idx = arith.constant 0 : index
  %cst = arith.constant 0.0 : f32

  // CHECK: %[[dummy:.*]] = "test.dummy_op"(%[[t1]])
  %0 = "test.dummy_op"(%t1) : (tensor<?xf32>) -> tensor<?xf32>

  // CHECK: %[[dummy_memref:.*]] = bufferization.to_memref %[[dummy]]
  // CHECK: %[[v1:.*]] = vector.transfer_read %[[dummy_memref]]
  %1 = vector.transfer_read %0[%idx], %cst : tensor<?xf32>, vector<5xf32>

  // CHECK: %[[another_dummy:.*]] = "test.another_dummy_op"(%[[dummy]])
  %2 = "test.another_dummy_op"(%0) : (tensor<?xf32>) -> tensor<?xf32>

  // CHECK: return %[[v1]], %[[another_dummy]]
  return %1, %2 : vector<5xf32>, tensor<?xf32>
}

// -----

// CHECK-LABEL: func @use_of_bufferizable_op_in_unbufferizable_op
//  CHECK-SAME:     %[[t1:.*]]: tensor<?xf32>
func.func @use_of_bufferizable_op_in_unbufferizable_op(
    %t1: tensor<?xf32>, %o: index, %s: index) -> (tensor<?xf32>, tensor<?xf32>) {
  // CHECK: %[[m1:.*]] = bufferization.to_memref %[[t1]]
  // CHECK: %[[subview:.*]] = memref.subview %[[m1]]
  // The op must alloc because "test.dummy" may bufferize to a memory write.
  // CHECK: %[[alloc:.*]] = memref.alloc
  // CHECK: memref.copy %[[subview]], %[[alloc]]
  %0 = tensor.extract_slice %t1[%o][%s][1] : tensor<?xf32> to tensor<?xf32>
  // CHECK: %[[alloc_tensor:.*]] = bufferization.to_tensor %[[alloc]]
  // CHECK: %[[dummy:.*]] = "test.dummy_op"(%[[alloc_tensor]])
  %1 = "test.dummy_op"(%0) : (tensor<?xf32>) -> tensor<?xf32>
  // CHECK: return %[[alloc_tensor]], %[[dummy]]
  return %0, %1 : tensor<?xf32>, tensor<?xf32>
}

// -----

// CHECK-LABEL: func @unused_unknown_op(
//  CHECK-SAME:     %[[t1:.*]]: tensor<?xf32>
func.func @unused_unknown_op(%t1 : tensor<?xf32>) -> vector<5xf32> {
  %idx = arith.constant 0 : index
  %cst = arith.constant 0.0 : f32

  // CHECK: %[[m1:.*]] = bufferization.to_memref %[[t1]]
  // CHECK: vector.transfer_read %[[m1]]
  %1 = vector.transfer_read %t1[%idx], %cst : tensor<?xf32>, vector<5xf32>

  // CHECK: "test.dummy_op"(%[[t1]])
  "test.dummy_op"(%t1) : (tensor<?xf32>) -> ()

  return %1 : vector<5xf32>
}

// -----

// CHECK-LABEL: func @unknown_op_may_read(
func.func @unknown_op_may_read(%v: vector<5xf32>)
    -> (tensor<10xf32>, tensor<10xf32>) {
  %idx = arith.constant 0 : index
  %cst = arith.constant 5.0 : f32

  // One alloc for the alloc_tensor, another one because the transfer_write
  // bufferizes out-of-place.
  // CHECK: %[[m1:.*]] = memref.alloc() {{.*}} : memref<10xf32>
  // CHECK: linalg.fill ins(%{{.*}}{{.*}}outs(%[[m1]]
  // CHECK: %[[filled_tensor:.*]] = bufferization.to_tensor %[[m1]]
  %t1 = bufferization.alloc_tensor() : tensor<10xf32>
  %filled = linalg.fill ins(%cst : f32) outs(%t1 : tensor<10xf32>) -> tensor<10xf32>

  // The transfer_write is out-of-place because "dummy_op" may read.
  // CHECK: %[[alloc:.*]] = memref.alloc() {{.*}} : memref<10xf32>
  // CHECK: memref.copy %[[m1]], %[[alloc]]
  // CHECK: vector.transfer_write %{{.*}}, %[[alloc]]
  // CHECK: %[[alloc_tensor:.*]] = bufferization.to_tensor %[[alloc]]
  %1 = vector.transfer_write %v, %filled[%idx] : vector<5xf32>, tensor<10xf32>

  // CHECK: %[[dummy:.*]] = "test.dummy_op"(%[[filled_tensor]])
  %2 = "test.dummy_op"(%filled) : (tensor<10xf32>) -> (tensor<10xf32>)

  // CHECK: return %[[alloc_tensor]], %[[dummy]]
  return %1, %2 : tensor<10xf32>, tensor<10xf32>
}

// -----

// CHECK-LABEL: func @unknown_op_not_writable
//  CHECK-SAME:     %[[t1:.*]]: tensor<?xf32>
func.func @unknown_op_not_writable(
    %t1 : tensor<?xf32>, %v :  vector<5xf32>, %idx : index) -> tensor<?xf32> {
  // CHECK: %[[dummy:.*]] = "test.dummy_op"(%[[t1]])
  // CHECK: %[[dummy_memref:.*]] = bufferization.to_memref %[[dummy]]
  %0 = "test.dummy_op"(%t1) : (tensor<?xf32>) -> (tensor<?xf32>)

  // The result of an unknown op is not writable. Always generate a copy.
  // CHECK: %[[dim:.*]] = memref.dim %[[dummy_memref]]
  // CHECK: %[[alloc:.*]] = memref.alloc(%[[dim]])
  // CHECK: memref.copy %[[dummy_memref]], %[[alloc]]
  // CHECK: vector.transfer_write %{{.*}}, %[[alloc]]
  %1 = vector.transfer_write %v, %0[%idx] : vector<5xf32>, tensor<?xf32>

  // CHECK: %[[alloc_tensor:.*]] = bufferization.to_tensor %[[alloc]]
  // CHECK: return %[[alloc_tensor]]
  return %1 : tensor<?xf32>
}

// -----

// CHECK-TENSOR-LABEL: func @simple_tensor_test(
//  CHECK-TENSOR-SAME:     %[[t1:.*]]: tensor<?xf32>
func.func @simple_tensor_test(%t1 : tensor<?xf32>, %f : f32) -> tensor<?xf32> {
  // CHECK-TENSOR: %[[t1_memref:.*]] = bufferization.to_memref %[[t1]]
  %c0 = arith.constant 0 : index
  // CHECK-TENSOR: %[[alloc:.*]] = memref.alloc
  // CHECK-TENSOR: memref.copy %[[t1_memref]], %[[alloc]]
  // CHECK-TENSOR: memref.store %{{.*}}, %[[alloc]]
  %0 = tensor.insert %f into %t1[%c0] : tensor<?xf32>
  // CHECK-TENSOR: %[[casted_alloc:.*]] = bufferization.to_tensor %[[alloc]]
  // CHECK-TENSOR: return %[[casted_alloc]]
  return %0 : tensor<?xf32>
}

// -----

// CHECK-SCF-LABEL: func @simple_scf_if(
//  CHECK-SCF-SAME:     %[[t1:.*]]: tensor<?xf32> {bufferization.writable = true}, %[[c:.*]]: i1, %[[pos:.*]]: index
func.func @simple_scf_if(%t1: tensor<?xf32> {bufferization.writable = true}, %c: i1, %pos: index, %f: f32)
    -> (tensor<?xf32>, index) {
  // CHECK-SCF: %[[t1_memref:.*]] = bufferization.to_memref %[[t1]]
  // CHECK-SCF: %[[r:.*]] = scf.if %[[c]] -> (memref<?xf32, strided{{.*}}>) {
  %r1, %r2 = scf.if %c -> (tensor<?xf32>, index) {
    // CHECK-SCF: scf.yield %[[t1_memref]]
    scf.yield %t1, %pos : tensor<?xf32>, index
  // CHECK-SCF: } else {
  } else {
    // CHECK-SCF: %[[insert:.*]] = tensor.insert %{{.*}} into %[[t1]][{{.*}}]
    // CHECK-SCF: %[[insert_memref:.*]] = bufferization.to_memref %[[insert]]
    %1 = tensor.insert %f into %t1[%pos] : tensor<?xf32>
    // CHECK-SCF: scf.yield %[[insert_memref]]
    scf.yield %1, %pos : tensor<?xf32>, index
  }

  // CHECK-SCF: %[[r_tensor:.*]] = bufferization.to_tensor %[[r]]
  // CHECK-SCF: return %[[r_tensor]], %[[pos]]
  return %r1, %r2 : tensor<?xf32>, index
}
