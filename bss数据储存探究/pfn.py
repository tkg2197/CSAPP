"""
给定进程的pid与虚拟地址，输出虚拟地址对应的物理页是否存在、是否被交换、物理页号
"""
import sys, os

pid = sys.argv[1] #第一个参数是进程的pid
vaddr = int(sys.argv[2], 16) #第二个参数是虚拟地址
pgsz = os.sysconf("SC_PAGE_SIZE") #页大小，一般是4kb

offset = (vaddr // pgsz) * 8 # 虚拟页号
# 在进程的pagemap文件中查找对应的物理地址
with open(f"/proc/{pid}/pagemap", "rb") as f:
    f.seek(offset)
    entry = int.from_bytes(f.read(8), "little")

present = (entry >> 63) & 1
swapped = (entry >> 62) & 1
pfn = entry & ((1 << 55) - 1)
print(f"vaddr={vaddr:#x} present={present} swapped={swapped} pfn={pfn:#x}")
