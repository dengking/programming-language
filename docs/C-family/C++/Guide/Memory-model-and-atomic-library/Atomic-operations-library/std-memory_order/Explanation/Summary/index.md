对 memory order进行控制

## multi level: 从 范围来进行对比



release-consume ordering

对存在data dependency



release-acquire ordering

所有的，无论是否存在data dependency



sequentially consistent ordering

所有的、全局的

## Implementation

都会涉及strong、weak ordered memory CPU。