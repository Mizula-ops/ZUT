import cv2
import numpy as np
import sys
from tqdm import tqdm
img1 = cv2.imread("img1.jpg")
img1=img1.astype(int)
print(img1.shape)
img2 = cv2.imread("img2.jpg")
img2=img2.astype(int)
print(img2.shape)
img3 = cv2.imread("img3.jpg")
img3=img3.astype(int)
print(img3.shape)


import sys

def get_size(obj, seen=None):
    size = sys.getsizeof(obj)
    if seen is None:
        seen = set()
    obj_id = id(obj)
    if obj_id in seen:
        return 0
    seen.add(obj_id)
    if isinstance(obj,np.ndarray):
        size=obj.nbytes
    elif isinstance(obj, dict):
        size += sum([get_size(v, seen) for v in obj.values()])
        size += sum([get_size(k, seen) for k in obj.keys()])
    elif hasattr(obj, '__dict__'):
        size += get_size(obj.__dict__, seen)
    elif hasattr(obj, '__iter__') and not isinstance(obj, (str, bytes, bytearray)):
        size += sum([get_size(i, seen) for i in obj])
    return size

def kompresja(data):
    x=np.array([len(data.shape)])
    x=np.concatenate([x,data.shape])
    OGshape = x[1:int(x[0]+1)]
    print(OGshape)
    Array1D = data.flatten()

def rle_encode(data):
    data=data.astype(int)
    x = np.array([len(data.shape)])
    x = np.concatenate([x, data.shape])
    flat=data.flatten()
    n=len(flat)
    buffer=np.zeros((n*2))
    out_idx=0
    i=0
    pbar=tqdm(total=n)
    while i<n:
        start_i=i
        count=1
        for j in range(i+1,n):
            if(flat[j] == flat[i]):
                count+=1
            else:
                break
        buffer[out_idx]=count
        buffer[out_idx+1]=flat[i]
        out_idx+=2
        i+=count
        pbar.update(i-start_i)
    pbar.close()
    compressed_data = buffer[:out_idx]
    result= np.concatenate([x,compressed_data])

    return result.astype(int)

def rle_decode(data):
    OGshape = data[1:int(data[0] + 1)]
    encoded = data[int(data[0]) + 1:]
    decoded=[]
    for i in range (0,len(encoded),2):
        count=encoded[i]
        value=encoded[i+1]
        decoded.extend([value] * count)
    decoded=np.array(decoded)
    return decoded.reshape(OGshape).astype(int)
kompresja(img1)

def byteRun_encode(data):
    data=data.astype(int)
    x = np.array([len(data.shape)])
    x = np.concatenate([x, data.shape])
    flat=data.flatten()
    n=len(flat)

    buffer=np.zeros((n*2))
    out_idx=0
    i=0
    pbar = tqdm(total=n)
    while i<n:
        start_i = i
        if i<n-1 and flat[i]==flat[i+1]:
            count=1
            for j in range(i+1,n):
                if(flat[j] == flat[i]):
                    count+=1

                else:
                    break
            full_count=count
            while count > 128:
                buffer[out_idx] = -127
                buffer[out_idx + 1] = flat[i]
                out_idx += 2
                count -= 128

            buffer[out_idx]=1-count
            buffer[out_idx+1]=flat[i]
            out_idx+=2
            i+=full_count
        else:
            values=[flat[i]]
            for j in range(i+1,n):
                if j<n-1 and flat[j]==flat[j+1]:
                    break

                values.append(flat[j])
            count=len(values)
            idx=0
            while count > 128:
                buffer[out_idx] = 127
                out_idx += 1
                for k in range(128):
                    buffer[out_idx] = values[idx]
                    out_idx += 1
                    idx += 1
                count -= 128
            buffer[out_idx] = count-1
            out_idx+=1
            for k in range(count):
                buffer[out_idx] = values[idx]
                out_idx += 1
                idx += 1
            i+=len(values)
        pbar.update(i - start_i)
    pbar.close()
    compressed_data = buffer[:out_idx]
    result= np.concatenate([x,compressed_data])
    return result.astype(int)

def byterun_decode(data):
    OGshape = data[1:int(data[0] + 1)]
    encoded = data[int(data[0]) + 1:]
    decoded=[]
    i=0
    while i<len(encoded):
        count=encoded[i]
        if count<0:

            value=encoded[i+1]
            decoded.extend([value] * (1-count))
            i+=2
        else:
            decoded.extend(encoded[i+1:i+1+count+1])
            i += 1+ count+1
    decoded = np.array(decoded)
    return decoded.reshape(OGshape).astype(int)

test_data = [
    np.array([1,1,1,1,2,1,1,1,1,2,1,1,1,1]),
    np.array([1,2,3,1,2,3,1,2,3]),
    np.array([5,1,5,1,5,5,1,1,5,5,1,1,5]),
    np.array([-1,-1,-1,-5,-5,-3,-4,-2,1,2,2,1]),
    np.zeros((1,520)),
    np.arange(0,521,1),
    np.eye(7),
    np.dstack([np.eye(7),np.eye(7),np.eye(7)]),
    np.ones((1,1,1,1,1,1,10))
]

for idx, test in enumerate(test_data):
    test = test.astype(int)

    enc_rle = rle_encode(test)
    dec_rle = rle_decode(enc_rle)

    enc_br = byteRun_encode(test)
    dec_br = byterun_decode(enc_br)

    print(f"\n=== Test {idx+1} === Shape: {test.shape}")

    print("Oryginał:")
    print(test)

    print("\nRLE dekodowane:")
    print(enc_rle)

    print("\nByteRun dekodowane:")
    print(enc_br)

    print("\nRLE poprawne:", np.array_equal(test, dec_rle))
    print("RLE poprawne:", np.array_equal(test, dec_rle))
    print("ByteRun poprawne:", np.array_equal(test, dec_br))

images = [img1, img2, img3]
tekst=["Rysunek Techniczny","Faktura","Zdjecie"]
for idx, i in enumerate(images):
    enc1 = rle_encode(i)
    dec1 = rle_decode(enc1)

    enc2 = byteRun_encode(i)
    dec2 = byterun_decode(enc2)

    print(f"\n=== Obraz {tekst[idx]} ===")
    print("RLE poprawne:", np.array_equal(i, dec1))
    print("ByteRun poprawne:", np.array_equal(i, dec2))

    original_size = get_size(i)
    rle_size = get_size(enc1)
    br_size = get_size(enc2)

    print("Rozmiar oryginalny:", original_size)
    print("Rozmiar RLE:", rle_size)
    print("Rozmiar ByteRun:", br_size)

    CR_rle = original_size / rle_size
    CR_br = original_size / br_size

    PR_rle = (rle_size / original_size) * 100
    PR_br = (br_size / original_size) * 100

    print("CR RLE:", CR_rle)
    print("CR ByteRun:", CR_br)
    print("Procent RLE:", PR_rle, "%")
    print("Procent ByteRun:", PR_br, "%")


