# [Effective Concurrency: Prefer Using Active Objects Instead of Naked Threads](https://herbsutter.com/2010/07/12/effective-concurrency-prefer-using-active-objects-instead-of-naked-threads/)

 This month’s **Effective Concurrency** column, “[Prefer Using Active Objects Instead of Naked Threads](http://www.drdobbs.com/go-parallel/article/showArticle.jhtml;jsessionid=JM3XD1KM22SCRQE1GHPSKH4ATMY32JVN?articleID=225700095),” is now live on DDJ’s website. 

 From the article: 

> … **Active objects** dramatically improve our ability to reason about our thread’s code and operation by giving us higher-level abstractions and idioms that raise the semantic level of our program and let us express our intent more directly. As with all good patterns, we also get better vocabulary to talk about our design. Note that active objects aren’t a novelty: UML and various libraries have provided support for active classes. Some **actor-based languages** already have variations of this pattern baked into the language itself; but fortunately, we aren’t limited to using only such languages to get the benefits of active objects.
>
> This article will show how to implement the pattern, including a reusable helper to automate the common parts, in any of the popular mainstream languages and threading environments, including `C++`, C#/.NET, Java, and C/Pthreads.

 

> NOTE: 
>
> tag-active object is a good abstraction





## COMMENTS

###  [Kjell Hedström](http://www.kjellkod.cc/) 

Thanks for yet another great article Herb.

I think that instead of using **Message objects** (for pre `C++0x` developers) it could be benificial to follow the idiom of **generic function callbacks** ( your Gotw: 83 et.al ).

This way you can easily make the **Active object** and Backgrounder without **message objects** focus on the core with cleaner code as a result.

Inspired by your Effective Concurrency Europe 2009 seminar I ended up with something like this :)

I.e. : (cut-down & pseudo-ish version)


```C++
Class Active {
private:
….
void run() {
while(!done){
smart_ptr msg= mq.pop();
msg(); // executes msg
}
}

public:
….
~Active() {
send(bind(this, &Active::DoDone));
thd->join();
}
….
};

class Background {
…
public:
void Save(smart_ptr data) {
active->send(bind(this, &Background::DoSave, data);
}
…
};
```
Standard pre `C++0x` but still with the clean look thanks to some template touch :)

Cheers

