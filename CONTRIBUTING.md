Contributing    
============
- All contributions, bug reports, bug fixes, documentation improvements, enhancements, and ideas are welcome.
- Want to contribute? Great! First, read this page (including the small print at
the end).

# Before you contribute
Before we can use your code, you must sign the
[Google Individual Contributor License Agreement](https://developers.google.com/open-source/cla/individual?csw=1)
- (CLA), which you can do online. The CLA is necessary mainly because you own the copyright to your changes, even after your contribution becomes part of our codebase, so we need your permission to use and distribute your code. We also need to be sure of various other things—for instance that you'll tell us if you know that your code infringes on other people's patents. You don't have to sign the CLA until after you've submitted your code for review and a member has approved it, but you must do it before we can put your code into our codebase. Before you start working on a larger contribution, you should get in touch with us first through the issue tracker with your idea so that we can help out and possibly guide you. Coordinating up front makes it much easier to avoid frustration later on.

# Code reviews
- All submissions, including submissions by project members, require review. We use Github pull requests for this purpose.

# The small print
- Contributions made by corporations are covered by a different agreement than the one above, the Software Grant and Corporate Contributor License Agreement.

# Forking
You will need your own fork to work on the code. Go to the [mathfu](https://github.com/google/mathfu) project page and hit the ***Fork*** button. You will want to clone your fork to your machine:
```
git clone https://github.com/<your-username>/mathfu.git <your-new-branch-name>
cd <your-new-branch-name>
git remote add upstream https://github.com/google/mathfu.git
```

# Creating a Branch
You want your master branch to reflect only production-ready code, so create a feature branch for making your changes. For example:
```
git branch <your-new-branch-name>
git checkout <your-new-branch-name>
```
This changes your working directory to the <your-new-branch-name> branch. Keep any changes in this branch specific to one bug or feature so it is clear what the branch brings to mathfu.

# Committing your code
Once you’ve made changes, you can see them by typing:
```
git status
```
If you have created a new file and it is not being tracked by git. Add it by typing:
```
git add path/to/file-to-be-added
```

# Pushing your changes
When you want your changes to appear publicly on your GitHub page, push your forked feature branch’s commits:
```
git push origin <your-new-branch-name>
```