from bs4 import BeautifulSoup
import requests
import re

def fetch_page(page_url):
    try:
        response = requests.get(page_url)
    except:
        return None

    if response.status_code == 200:
        return response.text
    
    return None

def process_page_content(page_content: str):
    soup = BeautifulSoup(page_content, 'html.parser')
    
    page_dict = {}
    unique_tags = set([tag.name for tag in soup.find_all()])
    
    for tag in unique_tags:
        page_dict[tag] = []
    
    for tag in soup.find_all():
        page_dict[tag.name].append(process_text(tag.text))
    
    return page_dict

def process_text(text: str):
    return re.sub(r'\n', ' ', text.lower())

if __name__ == "__main__":
    url_list = ["https://en.wikipedia.org/wiki/Bash_(Unix_shell)"]
    
    for url in url_list:
        page_content = fetch_page(url)
        processed_page = process_page_content(page_content)
        
        print(processed_page)