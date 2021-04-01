@mainpage FastPL

To get started on the project, all the instructions are linked into the README:
@subpage readme

To get started on Olivier's thesis, here is the documentation related to our implementation of the metaheuristic:
@subpage learning_algorithms

# Fast Preference Learning

## Abstract 

This documentation will allow you to understand the main thread of Olivier Sobries' metaheuristic for majority rule sorting (MR-Sort) models. Traditional methods to learn MR-Sort model parameters tend to be computationally inefficient when datasets grow larger. The metaheurisitc enables us to learn a majority rule sorting model from large datasets. 

## Introduction

Before going into the details of the algorithm let us frame the contextual and theorical landscape of the project. Any details of all the information that follows can be found in **Olivier Sobrie's thesis**. It is highly encouraged to go thoroughly through his work.

## Multiple-Criteria Decision Analysis : MCDA
In operations research, multiple-criteria decision analysis (MCDA) aims at helping users confronted to decision problems by the use of formal models. In MCDA, the user is also called decision maker (DM). While MCDA methods do not aim to substitute the DM and make decision on his/her behalf, they allow to structure the decision problem and give an insight into the DM’s preferences. Decision problems considered in MCDA are of different types. For instance, a municipality has to choose the location of a new sports facility. This problem can be formulated as a choice problem in which the best solution is to be selected. MCDA aims at answering several types of decision problems. Roy (1985, 1996) identified the following three types of decision problems : **Choice problem**, **Ranking problem** and **Sorting problem**.

### Two families of MCDA methodologies
MCDA methods are generally classified in two families. The first family encompasses methods based on multi-attribute value theory (MAVT) (Keeney and Raiffa, 1976) while the second includes methods based on outranking relations (Roy, 1991). 

They are many outranking methods but Roy and Bouyssou (1993) developed ELECTRE TRI which allows to deal with sorting problems.
**As matter of fact, majority rule sorting models are a simplified version of ELECTRE TRI.**

## MCDA Definition 

**Criterion** and **alternative** are two important notions in MCDA. We recall here their definitions.
In MCDA, a decision maker is often confronted to different solutions to a problem. These solutions are also called alternatives or actions.

*An **alternative** “a” is the representation of a possible contribution to the comprehensive decision that can be considered autonomously with respect to the decision process development state and that can serve as an application point for the decision aid. The application point is, then, sufficient to characterize “a”.*

In MCDA, the alternatives are evaluated on multiple attributes, called criteria. A criterion is a function that assigns an evaluation to each alternative involved in the decision problem. The coding of a criterion is often made such that the preference either increases or decreases as a function of the criterion value. The preference scale associated with a criterion should be monotone and each criterion should have a preference direction: the value of the criterion should be either minimized or maximized. For instance, in the context of choosing a car, disregarding all other criteria, a DM usually prefers a cheap car to an expensive one. 

*A **criterion** is a function \f$g_j\f$ assigning a value to each alternative of a decision problem such that it becomes possible for a DM to perform pairwise comparisons and express a preference in favor of one alternative based on this value.*

The Cartesian product of criteria scales is denoted by  X = \f$\prod_{j=1}^{n}\f$ \f$X_j\f$ in which \f$X_j\f$ represents all the possible values of a criterion j. 
We designate \f$g_j\f$ : \f$A\f$ \f$\to\f$ \f$X_j\f$ the function assigning a value to each alternative of the set \f$A\f$. \f$A\f$ represents a finite set of alternatives for which a ranking or a sorting has to be determined. Let \f$a\f$ \f$\in\f$ \f$A\f$ be an alternative \f$a\f$ contained in the set \f$A\f$. We denote by \f$a_j\f$ = \f$g_j(a)\f$ the performance of an alternative a on criterion \f$j\f$. The performance vector associated to each alternative a is denoted by \f$a\f$ = (\f$a_1\f$, ..., \f$a_n\f$);

There exist different **types of preference relation** between two alternatives. Roy and Bouyssou (1993) list four types of preferences:
- *Indifference*: the two alternatives are considered equivalent;
- *Strict preference*: one alternative is considered as strictly preferred to another
one;
- *Weak preference*: the set of arguments in favor of one alternative against
another is not strong enough to say that the first alternative is preferred to
the second;
- *Incomparability*: two alternatives cannot be compared because their evaluations
are very contrasted.

According to the preference of the DM on the set of alternatives in \f$A\f$, those alternatives can be assigned into **categories**.
## ELECTRI TRI

ELECTRI TRI is an outranking sorting procedure. The method aims at assigning each alternative of a set to a category selected among a set of pre-defined and ordered categories. Each alternative in the set is evaluated on a set of monotone criteria. The category in which an alternative is assigned to is chosen by comparing the alternative performances to the performances of profiles delimiting the categories.

Formally we denote by A the set of alternatives that have to be assigned to a category selected among p ordered categories, \f$C^p\f$  \f$\succ\f$  \f$C^{p-1}\f$   \f$\succ\f$ ...   \f$\succ\f$ \f$C^1\f$ where \f$\succ\f$ indicates a binary outranking relation. Alternatives in the set are evaluated on multiple attributes which are known to have a monotone preference scale, i.e. the higher the value of an alternative on criterion j the better it is or the contrary. The set of criteria is denoted by N = \f$\{1,..., n\}\f$ and j denotes one of the criteria in the set N. To know in which category an alternative should be assigned, its performances are compared to the ones of the p-1 profiles delimiting the p categories. We denote by \f$b_h\f$ the profile delimiting the category \f$C^h\f$ from \f$C^{h+1}\f$. H = \f$\{1,..., p-1\}\f$ denotes the set of profile indices. Figure 2.1 illustrates the profiles and categories of an ELECTRE TRI model. Alternatives are compared to the profiles delimiting the categories according a specific schema given by the ELECTRE TRI parameter learning model. Formally, an outraking relation is given to ELECTRE TRI in order for alternatives to be assigned to specific categories. I encourage to check out the thesis page 13.


<img src="../images/Electre_Tri_model.png"  width="600"/>

## MR-Sort Model

In this model, an alternative \f$a\f$ outranks a profile \f$b_h\f$ if the following two conditions are
satisfied:
1. \f$a\f$ has better performances than \f$b^h\f$ on a sufficiently large coalition of criteria;
2. \f$a\f$ is not significantly worse than \f$b^h\f$ on any criterion \f$j \in N\f$.

With MR-Sort we assume that “sufficiently large coalitions of criteria” can be determined by a weight \f$w_j\f$ associated with each criterion and a majority threshold \f$\lambda\f$. A coalition of criteria \f$J\f$ is “sufficiently large” if   \f$\sum_{j=1}^{n}\f$ \f$w_j\f$ > \f$\lambda\f$ .Formally, the outranking relation of MR-Sort reads:

\f$a \succ b^h \Leftrightarrow \sum_{j: a_j \geq b_j^h} w_j \geq \lambda\f$ and \f$!\exists j \in N : a_j< b_j^h - v_j^h\f$

with \f$v_j^h\f$ the veto profile associated to the profile \f$b^h\f$ on criterion \f$j\f$.

However, the thesis and the metaheuristic only deals with **MRSort models without veto**. Therefore, the outranking relation boils down to : 

\f$a \succ b^h \Leftrightarrow \sum_{j: a_j \geq b_j^h} w_j \geq \lambda\f$ and \f$ !\exists j \in N : a_j< b_j^h\f$

Using the equation above, the asssignment rule given is as follows : 

\f$a \in C^h \Leftrightarrow \sum_{j: a_j \geq b_j^{h-1}} w_j \geq \lambda\f$ and \f$\sum_{j: a_j \geq b_j^h} w_j < \lambda\f$

### Traditional way of learning the parameters of MR-Sort models

Learning all of parameters is far from trivial. Most of the time the problem is broken down in subproblems. The first one is finding the weights and the cutting threshold of an ELECTRE TRI model with fixed profiles and indifference and preference thresholds. In the paper, a linear program (LP) is proposed and some experiments are conducted.  Then, mixed integer program (MIP) in order to infer the profiles of an ELECTRE TRI model with fixed weights and thresholds. Please refer to page 37 of the thesis for more information.

## Metaheuristic algorithm elicitation

Olivier's thesis proposed a metaheuristic to elicite MRSort model without vetoes parameters.

To get started on Olivier's thesis, here is the documentation related to our implementation of the metaheuristic:
@subpage learning_algorithms




