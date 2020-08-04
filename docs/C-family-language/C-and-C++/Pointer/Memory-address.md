# [Memory address](https://en.wikipedia.org/wiki/Memory_address)

In [computing](https://en.wikipedia.org/wiki/Computing), a **memory address** is a reference to a specific [memory](https://en.wikipedia.org/wiki/Computer_memory) location used at various levels by [software](https://en.wikipedia.org/wiki/Software) and [hardware](https://en.wikipedia.org/wiki/Computer_hardware). Memory addresses are fixed-length sequences of [digits](https://en.wikipedia.org/wiki/Numerical_digit) conventionally displayed and manipulated as [unsigned](https://en.wikipedia.org/wiki/Signedness) [integers](https://en.wikipedia.org/wiki/Integer_(computing)).[[1\]](https://en.wikipedia.org/wiki/Memory_address#cite_note-1)Such numerical semantic bases（基于） itself upon features of CPU (such as the [instruction pointer](https://en.wikipedia.org/wiki/Instruction_pointer) and incremental [address registers](https://en.wikipedia.org/wiki/Memory_address_register)), as well upon use of the memory like an [array](https://en.wikipedia.org/wiki/Array_data_structure) endorsed by various [programming languages](https://en.wikipedia.org/wiki/Programming_language).



## Types of memory addresses

### Physical addresses

A [digital computer](https://en.wikipedia.org/wiki/Digital_computer)'s [main memory](https://en.wikipedia.org/wiki/Main_memory) consists of many **memory locations.** Each **memory location** has a **physical address** which is a **code**. The CPU (or other device) can use the code to access the corresponding memory location. Generally only [system software](https://en.wikipedia.org/wiki/System_software), i.e. the [BIOS](https://en.wikipedia.org/wiki/BIOS), operating systems, and some specialized [utility programs](https://en.wikipedia.org/wiki/Utility_program) (e.g., [memory testers](https://en.wikipedia.org/wiki/Memory_tester)), address(定位) physical memory using machine code [operands](https://en.wikipedia.org/wiki/Operand#Computer_science) or [processor registers](https://en.wikipedia.org/wiki/Processor_register), instructing the CPU to direct a **hardware device**, called the [memory controller](https://en.wikipedia.org/wiki/Memory_controller), to use the [memory bus](https://en.wikipedia.org/wiki/Memory_bus) or [system bus](https://en.wikipedia.org/wiki/System_bus), or separate [control](https://en.wikipedia.org/wiki/Control_bus), [address](https://en.wikipedia.org/wiki/Address_bus) and [data busses](https://en.wikipedia.org/wiki/Bus_(computing)), to execute the program's commands. The memory controllers' [bus](https://en.wikipedia.org/wiki/Bus_(computing)) consists of a number of [parallel](https://en.wikipedia.org/wiki/Parallel_communication) lines, each represented by a [binary digit](https://en.wikipedia.org/wiki/Binary_digit) (bit). The width of the bus, and thus the number of addressable storage units, and the number of bits in each unit, varies among computers.

### Logical addresses

A [computer program](https://en.wikipedia.org/wiki/Computer_program) uses memory addresses to execute [machine code](https://en.wikipedia.org/wiki/Machine_code), and to store and retrieve [data](https://en.wikipedia.org/wiki/Data_(computing)). In early computers logical and physical addresses corresponded, but since the introduction of [virtual memory](https://en.wikipedia.org/wiki/Virtual_memory) most [application programs](https://en.wikipedia.org/wiki/Application_program) do not have a knowledge of physical addresses. Rather, they address **logical addresses**, or [virtual addresses](https://en.wikipedia.org/wiki/Virtual_address_space), using the computer's [memory management unit](https://en.wikipedia.org/wiki/Memory_management_unit) and [operating system](https://en.wikipedia.org/wiki/Operating_system) memory mapping; see [below](https://en.wikipedia.org/wiki/Memory_address#Address_space_in_application_programming).

​	

## Unit of address resolution

See also: [Word (computer architecture)](https://en.wikipedia.org/wiki/Word_(computer_architecture)) and [Binary prefix § Main memory](https://en.wikipedia.org/wiki/Binary_prefix#Main_memory)

Most modern computers are *byte-addressable*. Each address identifies a single [byte](https://en.wikipedia.org/wiki/Byte) ([eight bits](https://en.wikipedia.org/wiki/Octet_(computing))) of storage. Data larger than a single byte may be stored in a sequence of consecutive addresses. There exist *word-addressable* computers, where the minimal addressable storage unit is exactly the processor's [word](https://en.wikipedia.org/wiki/Word_(computer_architecture)). For example, the [Data General Nova](https://en.wikipedia.org/wiki/Data_General_Nova) [minicomputer](https://en.wikipedia.org/wiki/Minicomputer), and the [Texas Instruments TMS9900](https://en.wikipedia.org/wiki/Texas_Instruments_TMS9900) and [National Semiconductor IMP-16](https://en.wikipedia.org/wiki/IMP-16) [microcomputers](https://en.wikipedia.org/wiki/Microcomputer) used 16 bit [words](https://en.wikipedia.org/wiki/Word_(computer_architecture)), and there were many [36-bit](https://en.wikipedia.org/wiki/36-bit) [mainframe computers](https://en.wikipedia.org/wiki/Mainframe_computer) (e.g., [PDP-10](https://en.wikipedia.org/wiki/PDP-10)) which used 18-bit [word addressing](https://en.wikipedia.org/wiki/Word_orientation), not [byte addressing](https://en.wikipedia.org/wiki/Byte_addressing), giving an address space of 218 36-bit words, approximately 1 megabyte of storage. The efficiency of addressing of memory depends on the bit size of the bus used for addresses – the more bits used, the more addresses are available to the computer. For example, an 8-bit-byte-addressable machine with a 20-bit [address bus](https://en.wikipedia.org/wiki/Address_bus) (e.g. [Intel 8086](https://en.wikipedia.org/wiki/Intel_8086)) can address 220 (1,048,576) memory locations, or one [MiB](https://en.wikipedia.org/wiki/Mebibyte) of memory, while a 32-bit bus (e.g. [Intel 80386](https://en.wikipedia.org/wiki/Intel_80386)) addresses 232 (4,294,967,296) locations, or a 4 [GiB](https://en.wikipedia.org/wiki/Gigabyte) address space. In contrast, a 36-bit word-addressable machine with an 18-bit address bus addresses only 218 (262,144) 36-bit locations (9,437,184 bits), equivalent to 1,179,648 8-bit bytes, or 1152 KB, or 1.125 [MiB](https://en.wikipedia.org/wiki/MiB)—slightly more than the 8086.

Some older computers ([decimal computers](https://en.wikipedia.org/wiki/Decimal_computer)), were *decimal digit-addressable*. For example, each address in the [IBM 1620](https://en.wikipedia.org/wiki/IBM_1620)'s [magnetic-core memory](https://en.wikipedia.org/wiki/Magnetic-core_memory) identified a single six bit [binary-coded decimal](https://en.wikipedia.org/wiki/Binary-coded_decimal) digit, consisting of a [parity bit](https://en.wikipedia.org/wiki/Parity_bit), [flag bit](https://en.wikipedia.org/wiki/Flag_bit) and four numerical bits. The 1620 used 5-digit decimal addresses, so in theory the highest possible address was 99,999. In practice, the CPU supported 20,000 memory locations, and up to two optional external memory units could be added, each supporting 20,000 addresses, for a total of 60,000 (00000–59999).



## Contents of each memory location

See also: [binary data](https://en.wikipedia.org/wiki/Binary_data)

Each memory location in a [stored-program computer](https://en.wikipedia.org/wiki/Stored-program_computer) holds a [binary number](https://en.wikipedia.org/wiki/Binary_number) or [decimal number](https://en.wikipedia.org/wiki/Decimal_number) *of some sort*. Its interpretation, as data of some [data type](https://en.wikipedia.org/wiki/Data_type) or as an instruction, and use are determined by the [instructions](https://en.wikipedia.org/wiki/Instruction_(computer_science)) which retrieve and manipulate it.

Some early programmers combined instructions and data in words as a way to save memory, when it was expensive: The [Manchester Mark 1](https://en.wikipedia.org/wiki/Manchester_Mark_1) had space in its 40-bit words to store little bits of data – its processor ignored a small section in the middle of a word – and that was often exploited as extra data storage.[*citation needed*] [Self-replicating](https://en.wikipedia.org/wiki/Self-replicating) programs such as [viruses](https://en.wikipedia.org/wiki/Computer_virus) treat themselves sometimes as data and sometimes as instructions. [Self-modifying code](https://en.wikipedia.org/wiki/Self-modifying_code) is generally [deprecated](https://en.wikipedia.org/wiki/Deprecated) nowadays, as it makes testing and maintenance disproportionally difficult to the saving of a few bytes, and can also give incorrect results because of the compiler or processor's assumptions about the machine's [state](https://en.wikipedia.org/wiki/State_(computer_science)), but is still sometimes used deliberately, with great care.



### Address space in application programming

In modern [multitasking](https://en.wikipedia.org/wiki/Computer_multitasking) environment, an [application](https://en.wikipedia.org/wiki/Application_program) [process](https://en.wikipedia.org/wiki/Process_(computing)) usually has in its address space (or spaces) chunks of memory of following types:

- [Machine code](https://en.wikipedia.org/wiki/Machine_code), including:
  - program's own code (historically known as *code segment* or *text segment*);
  - [shared libraries](https://en.wikipedia.org/wiki/Shared_libraries).
- [Data](https://en.wikipedia.org/wiki/Data_(computing)), including:
  - initialized data ([data segment](https://en.wikipedia.org/wiki/Data_segment));
  - [uninitialized (but allocated)](https://en.wikipedia.org/wiki/.bss) variables;
  - [run-time stack](https://en.wikipedia.org/wiki/Run-time_stack);
  - [heap](https://en.wikipedia.org/wiki/Heap_(programming));
  - [shared memory](https://en.wikipedia.org/wiki/Shared_memory_(interprocess_communication)) and [memory mapped files](https://en.wikipedia.org/wiki/Memory_mapped_file).

Some parts of address space may be not mapped at all.



***SUMMARY*** : address space in application将指令（machine code）和data分开来进行存储，但是在底层，是不区分的；

## Addressing schemes

Main article: [Addressing mode](https://en.wikipedia.org/wiki/Addressing_mode)

A computer program can access an address given **explicitly** – in low-level programming this is usually called an **absolute address**, or sometimes a **specific address**, and is known as [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) data type in higher-level languages. But a program can also use [relative address](https://en.wikipedia.org/wiki/Relative_address) which specifies a location in relation to somewhere else (the *base address*). There are many more **indirect** [addressing modes](https://en.wikipedia.org/wiki/Addressing_mode).

Mapping logical addresses to physical and virtual memory also adds several levels of indirection; see below.

