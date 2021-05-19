from django.views.decorators.http import require_http_methods
from django.http import JsonResponse
import json
from .service import process

# Create your views here.

@require_http_methods(["GET", "POST"])
def aep_receiver(request):
    response = {}
    # try:
    if request.method == 'GET':
        response['msg'] = 'method is GET'
        return JsonResponse(response)
    json_result = json.loads(request.body)
    print(request.body)
    process.decode_json(json_result)
        
    # except Exception as e:
    #     response['msg'] = str(e)
    #     response['ok'] = 0
    return JsonResponse(response)
