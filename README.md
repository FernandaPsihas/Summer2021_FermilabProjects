# Summer2021_FermilabProjects
Summer2021 Fermilab Projects Resources and LogBook 

## Computing needs
Make sure you have the software/basic knowledge of the following items:

#### Github 
- Start a new account if you don't already have one. 
- Go over [github basics](https://guides.github.com/activities/hello-world/)

#### Code basics
You will use bash to navigate directories in the fermilab machines, and likely for some light scripting. 
- Go over [bash basics](https://towardsdatascience.com/basics-of-bash-for-beginners-92e53a4c117a)

- Go over some C++ basics if you're new to it, or reference guides like [this one](https://www.cplusplus.com/doc/tutorial/) as needed

*Root* is a C++ framework used to for analysis in particle physics. [Root website & resources](https://root.cern/) 
- [This](https://root.cern.ch/root/htmldoc/guides/primer/ROOTPrimer.html) is a good resource for root classes and code examples.  

You'll likely use *Python* at some point for handling data. 
- It is a good idea to be familiar with python basics and [matplotlib](https://matplotlib.org/stable/tutorials/index.html). 

LaTex is a document preparation system we often use for technical notes, publishable articles, etc. 
- Get familiar with the [basics of LaTeX](https://www.latex-project.org/)
- Have access to an editor [overleaf](https://www.overleaf.com/) is a good online colaborative editor.

- Have access to software for making slides (Keynote, Google slides, Power Point, Beamer). 
*You can read some tips on how to make good presentations [here](http://fernandapsihas.com/pdfs/YNTutorialHowToMakeTalks.pdf)*

- Have access to software for making posters (Keynote, Google slides, Power Point, Beamer)
*You can read some tips on how to make good poster [here](http://fernandapsihas.com/pdfs/201806PosterTutorial.pdf)*

#### Writing Clean Code 
Writing good code is essential for collaborative work (and good documentation). Take a look at the following guides for good coding practices: 
- [Code Hygiene](https://anishmahapatra.medium.com/code-hygiene-dont-laugh-it-off-2a5aebcdd84b)
- [Tips to write better code](https://www.geeksforgeeks.org/7-tips-to-write-clean-and-better-code-in-2020/)
- [Why should I write clean code?](https://blog.echobind.com/why-should-i-write-clean-code-6068548dbd7e)


#### Kerberos principal
Feel free to contact the [Fermilab service desk](https://fermi.servicenowservices.com/wp) if you have issues, or ask us directly. 
- You will need both the [kerberos principal](https://fermi.servicenowservices.com/kb_view.do?sysparm_article=KB0011306) and the correct .ssh_config settings on your machine to be able to connect to the gpvms. 
- Access to gpvms (for the corresponding experiment for your project)

## Communications 
- Make sure you have a Slack account or ask us more about it. 
- You can reference [this guide](https://slack.com/help/categories/360000049063#slack-101) if you are new to slack 
- Make sure you have access to our [google drive](https://drive.google.com/drive/folders/1UxBNgoPRD8kPxDypgZ2RC8pj6RG8TFJ7?usp=sharing) 
- Upload your slides to the meeting folder (or create one first if it doesn't exist)

## Getting Started
Here is a description on how to log in to the Fermilab computers and how to get 
started with your own directories.

#### Logging On
Follow these commands on your terminal to access the computers: 
- kinit -f <your_principle>@FNAL.GOV
- ssh -Y -X <your_principle>@uboonegpvm01.fnal.gov 
- Now you should be on uboonegpvm01 (the number indicates which computer node you are on)

#### Your Workspace
Run the following commands to get to the right place and organize your workspace. I 
recommend running these all at least once, and then having them run on startup.
- cd /uboone/app/users/<your_principle>/
- mkdir FirstTests
- Use one of the following to copy the GitHub repository, the first didn't work for me but has worked for others:
  git clone git@github.com:FernandaPsihas/Summer2021_FermilabProjects.git
  
  or

  git clone https://github.com/FernandaPsihas/Summer2021_FermilabProjects.git 

- cd Summer2021_FermilabProjects
- mkdir MicroBooNE_RadonDoping
- cd MicroBooNE_RadonDoping
- cp ../README.md .
- mkdir (yourprinciple)_firstplots

#### Aliasing and Start on Log-in
Here is some guidance on how to have commands run on log-in and how to alias. An alias is just a command nickname.
- pico ~/.profile
- Start adding commands such as cd /uboone/app/users/<your_principle>
- Add an alias if you like: alias root='root -l' 

#### Commiting Your Changes
Here are the commands needed for commiting changes to GitHub:
- git status (see what you have changed)
- git diff (check what you are commiting)
- git add <file_name> (add what you want to commit)
- git commit -m <message>
- git push origin main (push to the online repository)

#### Emacs Tutorials
  https://www.youtube.com/watch?v=NXqZWfQRo8M
  https://www.youtube.com/watch?v=E3y-0q4BVwA
  https://www.youtube.com/watch?v=GMQNjn7Bf2w
  https://www.gnu.org/software/emacs/refcards/pdf/refcard.pdf
  

