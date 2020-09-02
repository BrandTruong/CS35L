#!/usr/bin/python3

Brandon Truong

import os, sys, zlib

#from assignment webpage skeleton
class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()

#first implementation
def find_git_dir(path):
    looking_for = ".git"
    #current dir
    directory = os.path.abspath(path)
    while directory != "/": #loops until reaches / dir or not readable, stops after
        #temp to find
        temp = os.path.join(directory, looking_for)
        if os.path.isdir(temp):
            return temp
            #found .git dir, returns .git path
        #get parent dir
        directory = os.path.abspath(os.path.join(directory, os.pardir))
    #can't find
    sys.stdout.write("Not inside a git repository\n")
    sys.exit(1)

#second implementation
def get_branch_names(git_path):
    path = git_path + "/refs/heads/"
    head_length = len(path)
    dictionary = {} #store unordered keys=branches, values=hashes
    #invalid refs/heads folder
    if not os.path.isdir(path):
        sys.stdout.write("Can't find starting head folder\n")
        sys.exit(1)
    #Loop around directories, incl subdir
    for path, subdir_names, filenames in os.walk(path, topdown=True):
        #filesnames contain everything in current path, path is looped to subdir
        for names in filenames:
            #read file
            commit_hash = open(os.path.join(path,names), "r").read().strip("\n")
            branch_name = os.path.join(path,names)[head_length:] #substring out after /heads/
            #should be commit_hash always unique
            if not commit_hash in dictionary:
                #New commit_hash
                dictionary[commit_hash] = []
            #append branch_name based on fifth part implementation
            dictionary[commit_hash].append(branch_name)
    return dictionary

#third implementation
def build_commit_graph(git_path, dictionary):
    objects = git_path + '/objects/'
    commit_nodes = {}
    root_hashes = set()
    #What should these variables be initialized to?

    #use for loop through dictionary instead of visited to check cause im lazy
    for commit_hash in dictionary:
        #not yet visited
        if commit_hash not in commit_nodes:
            commit_nodes[commit_hash] = CommitNode(commit_hash)
            stack = [commit_nodes[commit_hash]]
            #stack loop including appended nodes
            while stack:
                #Get the next element from stack and store in commit_hash, and remove it from stack
                commit_node = stack.pop()
                #Get parents
                temp_parents = []
                #Find commit_hash in the objects folder, decompress it, and get parent commits
                current_file  = zlib.decompress(open(os.path.join(objects, commit_node.commit_hash[0:2], commit_node.commit_hash[2:]), 'rb').read())
                output = str(current_file, 'utf-8', 'strict')
                output_list = output.split("\n")

                #add stuff to parent list
                for current in output_list:
                    if "parent" in current:
                        temp_parents += (current.split()[1:])

                #change list to set
                commit_node.parents = set(sorted(temp_parents))

                if not commit_node.parents:
                    root_hashes.add(commit_node.commit_hash)
                else:
                    for p in commit_node.parents:
                    #What do we do if p isn’t in commit_nodes?
                        if p not in commit_nodes:
                            commit_nodes[p] = CommitNode(p)
                            parent = commit_nodes[p]
                            stack.append(parent)
                        #how do we record that commit_hash is a child of commit node p?
                        parent = commit_nodes[p]
                        parent.children.add(commit_node.commit_hash)

            root_hashes = sorted(root_hashes)
    
    root_hashes = sorted(root_hashes) #make sure output tree will look the same every time
    return commit_nodes, root_hashes



#fourth implementation
def get_topo_ordered_commits(commit_nodes, root_hashes):
    #temp_stack is kinda hard to implement
    order = []
    visited = set()
    stack = list(sorted(root_hashes))
    
    while stack:
        #find end first
        #last element in stack, don't pop until needed
        v = stack[-1]
        #we have seen this
        visited.add(v)
        end = True
        #loop to see if no more children for one part of tree
        for child in sorted(commit_nodes[v].children):
            #not the end, still undiscovered child
            if child not in visited:
                stack.append(child)
                end = False
                break
        #only start appending when we find all children of a part, aka least to greatest
        if end:
            order.append(stack.pop())
    return order

#fifth implementation from slides, prob not modifying cause I'm tired
def print_topo_ordered_commits_with_branch_names(commit_nodes, topo_ordered_commits, head_to_branches):
    jumped = False
    for i in range(len(topo_ordered_commits)):
        commit_hash = topo_ordered_commits[i]
        if jumped:
            jumped = False
            sticky_hash = ' '.join(commit_nodes[commit_hash].children)
            print(f'={sticky_hash}')
        branches = sorted(head_to_branches[commit_hash]) if commit_hash in head_to_branches else []
        print(commit_hash + (' ' + ' '.join(branches) if branches else ''))
        if i+1 < len(topo_ordered_commits) and topo_ordered_commits[i+1] not in commit_nodes[commit_hash].parents:
            jumped = True
            sticky_hash = ' '.join(commit_nodes[commit_hash].parents)
            print(f'{sticky_hash}=\n')

#helper main function to call all parts
def topo_order_commits():
    #First part
    repo = find_git_dir(".")
    #Second part
    branches = get_branch_names(repo)
    #Third part
    commit_nodes, root_hashes = build_commit_graph(repo, branches)
    #Fourth part
    ordered = get_topo_ordered_commits(commit_nodes, root_hashes)
    #Fifth part
    print_topo_ordered_commits_with_branch_names(commit_nodes, ordered, branches)

#from test case skeleton
if __name__ == '__main__':
    #haha my mental go boom boom
    topo_order_commits()

