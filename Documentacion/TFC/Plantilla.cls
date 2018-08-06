
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{book}}
\PassOptionsToClass{a4paper, 11pt, twoside, openright}{article}
\ProcessOptions\relax
\LoadClass{article}

\usepackage{fancyhdr} % Required for custom headers
\usepackage{lastpage} % Required to determine the last page for the footer
\usepackage{extramarks} % Required for headers and footers
\usepackage[usenames,dvipsnames]{color} % Required for custom colors
\usepackage{graphicx} % Required to insert images
\usepackage{listings} % Required for insertion of code
\usepackage{courier} % Required for the courier font
\usepackage{url}
\usepackage{mathptmx} % Times New Roman
\usepackage{multicol}


%----------------------------------------------------------------------------------------
% Change section style
%----------------------------------------------------------------------------------------
\usepackage{lipsum, graphicx, tabularx}
\usepackage[svgnames]{xcolor}
\usepackage[explicit]{titlesec}
\titleformat{\section}[block]
    {\bfseries\Huge\sffamily}
    {\rlap{\hspace*{\dimexpr\linewidth-5 \marginparsep}%
    \begin{tabular}{@{}>{\color{gray!90}}c@{}}\scalebox{4}{\thesection}\\[-1.3ex]%
    \rule{10pt}{10pt}\hspace{10pt}\rule{10pt}{10pt}\hspace{10pt}\rule{10pt}{10pt}\end{tabular}}}
    {0pt}
    {\color{gray!75}\begin{tabularx}{\dimexpr\linewidth+4\marginparsep}%
    {@{}>{\raggedright\arraybackslash}X>{\raggedleft\arraybackslash}r@{}}
       & \rule{0pt}{3ex}\\[0ex] #1
    \end{tabularx}}
    [\vspace{1ex}\titlerule\vspace{16pt}]

\titlespacing{\chapter}{0pt}{-5ex}{10ex}


%----------------------------------------------------------------------------------------
%	SPANISH LANGUAGE
%----------------------------------------------------------------------------------------
\usepackage{lmodern}
\usepackage[T1]{fontenc}
\usepackage[spanish,activeacute]{babel}
\usepackage[utf8]{inputenc} % Para poder incluir tildes y ñ automáticamente

%----------------------------------------------------------------------------------------
%	TODOs AND NOTES
%----------------------------------------------------------------------------------------

\usepackage{xargs}                      % Use more than one optional parameter in a new commands
\usepackage[colorinlistoftodos,prependcaption,textsize=tiny]{todonotes}
\newcommandx{\unsure}[2][1=]{\todo[linecolor=red,backgroundcolor=red!25,bordercolor=red,#1]{#2}}
\newcommandx{\change}[2][1=]{\todo[linecolor=blue,backgroundcolor=blue!25,bordercolor=blue,#1]{#2}}
\newcommandx{\info}[2][1=]{\todo[linecolor=OliveGreen,backgroundcolor=OliveGreen!25,bordercolor=OliveGreen,#1]{#2}}
\newcommandx{\improvement}[2][1=]{\todo[linecolor=Plum,backgroundcolor=Plum!25,bordercolor=Plum,#1]{#2}}
\newcommandx{\thiswillnotshow}[2][1=]{\todo[disable,#1]{#2}}

%----------------------------------------------------------------------------------------
%	MARGIN, HEADER AND FOOTER
%----------------------------------------------------------------------------------------
% Margins
\usepackage{lipsum}

\topmargin=-0.45in
\evensidemargin=0in
\oddsidemargin=0in
\textwidth=6.5in
\textheight=9.0in
\headsep=0.25in

\linespread{1.1} % Line spacing

% Set up the header and footer
\fancypagestyle{insection}{
\fancyhf{}
\fancyfoot[C]{[ \textbf{\thepage}   ]}
\renewcommand{\headrulewidth}{0pt}
\renewcommand{\footrulewidth}{1pt}
}

% First section page
\fancypagestyle{notsection}{

\fancyhead[LE,RO]{\textbf{\leftmark}}
\fancyhead[LO,RE]{[ \textbf{\thepage}   ]}
\fancyfoot[CE,LE,RE,CO,LO,RO]{}
\renewcommand{\headrulewidth}{1pt}
\renewcommand{\footrulewidth}{0pt}
}

\setlength\parindent{0pt} % Removes all indentation from paragraphs


%----------------------------------------------------------------------------------------
%	INFO BOXES
%----------------------------------------------------------------------------------------
\usepackage[tikz]{bclogo}
\usepackage[framemethod=tikz]{mdframed}
\usepackage[many]{tcolorbox}

\definecolor{bgblue}{RGB}{245,243,253}
\definecolor{ttblue}{RGB}{91,194,224}


\newtcolorbox{normalbox}[1][]{
  breakable,
  title=#1,
  colback=white,
  colbacktitle=white,
  coltitle=black,
  fonttitle=\center\bfseries,
  bottomrule=0pt,
  toprule=0pt,
  leftrule=3pt,
  rightrule=3pt,
  titlerule=0pt,
  arc=0pt,
  outer arc=0pt,
  colframe=black,
}

\newtcolorbox{greenbox}[1][]{
  breakable,
  freelance,
  title=#1,
  colback=green!10,
  colbacktitle=green!10,
  coltitle=black,
  fonttitle=\center\bfseries,
  bottomrule=0pt,
  boxrule=0pt,
  colframe=white,
  overlay unbroken and first={
  \draw[green!75!black,line width=3pt]
    ([xshift=5pt]frame.north west) -- 
    (frame.north west) -- 
    (frame.south west);
  \draw[green!75!black,line width=3pt]
    ([xshift=-5pt]frame.north east) -- 
    (frame.north east) -- 
    (frame.south east);
  },
  overlay unbroken app={
  \draw[green!75!black,line width=3pt,line cap=rect]
    (frame.south west) -- 
    ([xshift=5pt]frame.south west);
  \draw[green!75!black,line width=3pt,line cap=rect]
    (frame.south east) -- 
    ([xshift=-5pt]frame.south east);
  },
  overlay middle and last={
  \draw[green!75!black,line width=3pt]
    (frame.north west) -- 
    (frame.south west);
  \draw[green!75!black,line width=3pt]
    (frame.north east) -- 
    (frame.south east);
  },
  overlay last app={
  \draw[red!75!black,line width=3pt,line cap=rect]
    (frame.south west) --
    ([xshift=5pt]frame.south west);
  \draw[red!75!black,line width=3pt,line cap=rect]
    (frame.south east) --
    ([xshift=-5pt]frame.south east);
  },
}


%----------------------------------------------------------------------------------------
%	CODE INCLUSION CONFIGURATION
%----------------------------------------------------------------------------------------
\definecolor{gray97}{gray}{.97}
\definecolor{gray75}{gray}{.75}
\definecolor{gray45}{gray}{.45}
\definecolor{comments}{RGB}{0,100,0}

\lstset{ frame=Ltb,
     framerule=0pt,
     aboveskip=0.5cm,
     framextopmargin=3pt,
     framexbottommargin=3pt,
     framexleftmargin=0.4cm,
     framesep=0pt,
     rulesep=.4pt,
     backgroundcolor=\color{gray97},
     %
     stringstyle=\ttfamily\color{comments},
     showstringspaces = false,
     basicstyle=\footnotesize\ttfamily,
     commentstyle=\itshape\color{gray45},
	 keywordstyle=\bfseries\color{blue},
     %
     numbers=left,
     numbersep=15pt,
     numberstyle=\tiny,
     numberfirstline = false,
     breaklines=true,
   }
   
\lstdefinestyle{ConfigFiles}{% define own style
  language={[LaTeX]TeX},
  basicstyle=\small\ttfamily,
  linewidth=0.9\linewidth,
  breaklines=true,
  keywordstyle=\color{blue}\bfseries,
  identifierstyle=\color{orange},
  commentstyle=\color{cyan},
  backgroundcolor=\color{OliveGreen},
  tabsize=2,
  morekeywords = {parameter},
}

\lstdefinestyle{consola}
   {basicstyle=\scriptsize\bf\ttfamily,
    backgroundcolor=\color{gray75},
   }
 
\lstdefinestyle{C}
   {language=C,
   }
