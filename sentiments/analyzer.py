import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        #Create positives set data struct
        self.positives = set()
        #Open positives txt file in read mode
        file = open(positives, "r")
        #Itereate through lines (words)
        for line in file:
            #Ignore lines beginning with ";"
            if line.startswith(";") == False:
                #Remove leading/trailing spaces and add to positives set
                self.positives.add(line.strip())
        #Close positives txt file
        file.close()
        
        
        #Create negatives set data struct
        self.negatives = set()
        #Open negatives txt file in read mode
        file = open(negatives, "r")
        #Itereate through lines (words)
        for line in file:
            #Ignore lines beginning with ";"
            if line.startswith(";") == False:
                #Remove leading/trailing spaces and add to negative set
                self.negatives.add(line.strip())
        #Close negatives txt file
        file.close()
        

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        #Instantiate tokenizer 
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        
        #Initialize to sum score of each token (word)
        score = 0
        
        #Loop through each token (word) of tweet/arguement and apply score
        for token in tokens:
            if token.lower() in self.positives: 
                score = score + 1
            elif token.lower() in self.negatives:
                score = score - 1
            else:
                score = score
        #Return aggregate score of tweet/arguement
        return score
