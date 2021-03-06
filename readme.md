
# API_OSX_WIN

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/f499a83b6046485bb8465babd2ff3e5d)](https://app.codacy.com/app/raygarner13/api_osx_win_ios?utm_source=github.com&utm_medium=referral&utm_content=godratio/api_osx_win_ios&utm_campaign=Badge_Grade_Dashboard)

These are a collection of functions I have created to be used/developed for a game in production.
The purpose and target for them is similar to the stb libs.
Simple to integrate and easy to use.

If your looking for a more complete and well tested set of functionality
you might want to look <link>there<link> instead.


## Negatives
1. Still using some std libs.
2. Not tested throughouly.
3. api not stable.
4. Steps to integrate into other projects not decided.
5. Have not yet allowed for full c compatibility.
    5a. To be more specific you have to use the data types provided at the moment.

Most of these will evenutally be fixed any help is appreciated.

## Restrictions placed here

1. Memory allocations are explicit.
2. Single memory startup and try to maximize allocated memory reuse.
3. Simple as possible.
4. Single file includes if possible
5. Minimize the number of things user code has to do to get it to "just work"

While what we are providing here is not sigle header libraries just quite yet.  
We are aiming for that level of simplicity eventually.  For now its secondary to getting useable
apis that do the job.  

The following was Ripped from stb libs.  Same exact reasoning here.

#### Why single-file headers?

Windows doesn't have standard directories where libraries
live. That makes deploying libraries in Windows a lot more
painful than open source developers on Unix-derivates generally
realize. (It also makes library dependencies a lot worse in Windows.)

There's also a common problem in Windows where a library was built
against a different version of the runtime library, which causes
link conflicts and confusion. Shipping the libs as headers means
you normally just compile them straight into your project without
making libraries, thus sidestepping that problem.

Making them a single file makes it very easy to just
drop them into a project that needs them. (Of course you can
still put them in a proper shared library tree if you want.)

Why not two files, one a header and one an implementation?
The difference between 10 files and 9 files is not a big deal,
but the difference between 2 files and 1 file is a big deal.
You don't need to zip or tar the files up, you don't have to
remember to attach *two* files, etc.


#### What's the license?

These libraries are in the public domain. You can do anything you
want with them. You have no legal obligation
to do anything else, although I appreciate attribution.

