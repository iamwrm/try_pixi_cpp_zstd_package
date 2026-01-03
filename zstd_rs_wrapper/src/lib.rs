use std::ffi::c_int;
use std::ptr;
use std::slice;

/// Compress data using zstd
///
/// # Safety
/// - `src` must be valid for reads of `src_len` bytes
/// - `dst` must be valid for writes of `dst_capacity` bytes
/// - Returns the number of bytes written, or negative on error
#[no_mangle]
pub unsafe extern "C" fn zstd_rs_compress(
    dst: *mut u8,
    dst_capacity: usize,
    src: *const u8,
    src_len: usize,
    level: c_int,
) -> isize {
    if src.is_null() || dst.is_null() {
        return -1;
    }

    let src_slice = slice::from_raw_parts(src, src_len);
    let dst_slice = slice::from_raw_parts_mut(dst, dst_capacity);

    match zstd::bulk::compress_to_buffer(src_slice, dst_slice, level) {
        Ok(size) => size as isize,
        Err(_) => -1,
    }
}

/// Decompress zstd-compressed data
///
/// # Safety
/// - `src` must be valid for reads of `src_len` bytes
/// - `dst` must be valid for writes of `dst_capacity` bytes
/// - Returns the number of bytes written, or negative on error
#[no_mangle]
pub unsafe extern "C" fn zstd_rs_decompress(
    dst: *mut u8,
    dst_capacity: usize,
    src: *const u8,
    src_len: usize,
) -> isize {
    if src.is_null() || dst.is_null() {
        return -1;
    }

    let src_slice = slice::from_raw_parts(src, src_len);
    let dst_slice = slice::from_raw_parts_mut(dst, dst_capacity);

    match zstd::bulk::decompress_to_buffer(src_slice, dst_slice) {
        Ok(size) => size as isize,
        Err(_) => -1,
    }
}

/// Get the maximum compressed size for a given input size
#[no_mangle]
pub extern "C" fn zstd_rs_compress_bound(src_len: usize) -> usize {
    zstd::zstd_safe::compress_bound(src_len)
}

/// Get the decompressed size from a compressed frame
/// Returns 0 if unknown or error
#[no_mangle]
pub unsafe extern "C" fn zstd_rs_get_decompressed_size(
    src: *const u8,
    src_len: usize,
) -> usize {
    if src.is_null() {
        return 0;
    }

    let src_slice = slice::from_raw_parts(src, src_len);

    match zstd::zstd_safe::get_frame_content_size(src_slice) {
        Ok(Some(size)) => size as usize,
        _ => 0,
    }
}
