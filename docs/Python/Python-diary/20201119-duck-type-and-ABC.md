## python duck type and polymorphism

python的一个特点是无需声明类型，并且python还支持duck type，那么在python中声明一个ABC，然后派生出一系列的子类又有什么意义呢？

难道是虽然python不需要什么类型，但是是可以进行类型比较的，所以其实也是可以由用户来实现对类型的校验的，所以也能够达到类似c++中的类型检查（在编译期）；