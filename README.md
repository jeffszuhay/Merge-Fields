# Merge-Fields
A small, configurable merge-field program in C. Think email-merge.

I needed a small, fast program to create monthly dues reminders for a membership group I am in.
I like the idea of managing text files in Unix/Mac OS.

Sure, I could have used Microsoft Word's mail/merge; or, I could have used a Bash script; or 
I could have used some other scripting language. 
I started down that road and turned back. See the folder "0 Attempted use of Bash." <yuk>

Or I could have employed MailChimp. 
The problem there is I just don't know what they do with my email list; 
I simply don't trust anyone with my data. 
If you put your data on someone else's hard drive, is it truly secure/private?
Methinks not.

Since I am the author of <i>Learn C Programming</i>, I realized I had already written and tested
many of the facilities I'd need to create my own merge-field program.
Those programs can be found here on GitHub.
This repository represents the steps along the path from Bash script (abandoned) to a complete C 
program to merge fields from a data file with a template file into a single or multiple output files. 

These can then be used to send monthly emails to the list of members--no I'm not trying to create 
an email spamming app.
For that, you need your own mail server, time and knowledge to set it up and maintain it, and then 
protect it from the burning garbage heap that has become the open web. 
I just don't have time for that.

Here is the basic outline of development:

0-Attempted use of Bash scripting

1-Getting filenames from command line

2-Reading the template file line-by-line

3-Reading each line of template file into linked list

4-Reading field names and values

5-Merging values into lines & generating output

Each step will be self-contained in a sub-folder by that name. 

The goal of progressing in this manner is to illustrate one way to develop a complete program from 
start to finish. 
This method is basically: 
```
loop
  get a single feature working
  test it (verify it)
repeat until done
```

If you follow along, step by step, you should see a logical progression from beginning to end.
The rationale and choices for each step will be described elsewhere.
This repository will only show the successful result of each stage of development.

If time has passed, and you want to see the progression but I still have not given the rationale
elsewhere, I recommend that you go from one step to the next, perhaps a day at a time, using a
diff facility to see what changed between steps. 
Also, don't forget to play with various inputs and with the test programs.

The source code is free for you to use and modify as you see fit.
If you use this code, it would be polite if you gave me credit as the original author.
By doing so, however, you assume all responsibility for the use of this code
and release me from any liability for any outcome of your use of this code.
In other words, if you touched it or you broke it, then you bought it; it's yours.



