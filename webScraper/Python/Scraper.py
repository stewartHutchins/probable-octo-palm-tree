from urllib.request import urlopen as uReq
from bs4 import BeautifulSoup as soup


base_url = "https://ndb.nal.usda.gov/ndb/search/list?&qlookup="

def search_for_ingredients(search_term):
    search_term = search_term.replace(' ', '+')
    uClient = uReq(base_url + search_term)
    page_html = uClient.read()
    uClient.close()
    page_soup =  soup(page_html, "html.parser")
    print(page_soup)
    return page_soup

def check_no_items(page_soup):
    return page_soup.body.div.div.form.div


page_soup =search_for_ingredients("hey you")
print(check_no_items(page_soup))